#include <iostream>
#include <vector>
#include <iostream>
#include <time.h>
#include <rte_eal.h>
#include <rte_ethdev.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>
#include <random>

#include "itch_parser.hpp"
#include "benchmarks/benchmark_utils.hpp"
#include "benchmarks/example_benchmark.hpp"

std::atomic<bool> run_noise = true;

void allocator_noise() {
    std::mt19937 rng(123);
    std::uniform_int_distribution<int> size_dist(4096, 8*4096);
    std::uniform_int_distribution<int> action(0, 1);

    std::vector<void*> blocks;

    while (run_noise.load()) {
        if (action(rng) == 0 || blocks.empty()) {
            size_t sz = size_dist(rng);
            void* p = std::malloc(sz);
            memset(p, 0xAA, sz);
            blocks.push_back(p);
        } else {
            size_t i = rng() % blocks.size();
            std::free(blocks[i]);
            blocks.erase(blocks.begin() + i);
        }

        std::this_thread::sleep_for(std::chrono::nanoseconds(50));
    }
}


int main(int argc, char** argv) {
    int eal_argc = rte_eal_init(argc, argv);
    if (eal_argc < 0) {
        throw std::runtime_error("EAL init failed");
    }

    if (rte_eth_dev_count_avail() == 0) {
        throw std::runtime_error("Specify a vdev device");
    }

    uint16_t port_id = 0;
    rte_mempool* pool = rte_pktmbuf_pool_create(
        "mbuf_pool",
        8192,
        256,
        0,
        RTE_MBUF_DEFAULT_BUF_SIZE,
        rte_socket_id()
    );

    if (!pool) {
        throw std::runtime_error("mempool creation failed\n");
    }

    rte_eth_dev_info dev_info;
    int status = rte_eth_dev_info_get(port_id, &dev_info);
    if (status != 0) {
        throw std::runtime_error("failed to get device info\n");
    }

    struct rte_eth_conf conf = {};
    conf.txmode.offloads = dev_info.tx_offload_capa;
    conf.rxmode.offloads = dev_info.rx_offload_capa;

    if (rte_eth_dev_configure(port_id, 1, 1, &conf) < 0) {
        throw std::runtime_error("failed to configure the device\n");
    }

    rte_eth_txconf txconf = dev_info.default_txconf;
    txconf.offloads = conf.txmode.offloads;

    rte_eth_rxconf rxconf = dev_info.default_rxconf;
    rxconf.offloads = conf.rxmode.offloads;

    if (rte_eth_tx_queue_setup(port_id, 0, 1024, rte_socket_id(), &txconf)) {
        throw std::runtime_error("failed to configure the tx queue\n");
    }

    if (rte_eth_rx_queue_setup(port_id, 0, 1024, rte_socket_id(), &rxconf, pool)) {
        throw std::runtime_error("failed to configure the rx queue\n");
    }

    if (rte_eth_promiscuous_enable(port_id) != 0) {
        throw std::runtime_error("failed to enable promiscuous eth");
    }

    if (rte_eth_dev_start(port_id) < 0) {
        throw std::runtime_error("failed to start the device\n");
    }

    argc -= eal_argc;
    argv += eal_argc;

    std::string filepath;
    std::string outdir;

    if (argc != 3) {
        std::cout << "Please specify the file to parse and an output directory" << '\n';
        return 1;
    }

    filepath = argv[1];
    outdir   = argv[2];

    auto [src_buf, bytes_read] = init_benchmark(filepath);
    const std::byte* src = src_buf.data();
    size_t len = bytes_read;

    std::thread noise(allocator_noise);

    ITCH::ItchParser parser;
    BenchmarkOrderBook ob_bm_handler;
    rte_mbuf* bufs[32];

    while (true) {
        uint16_t n = rte_eth_rx_burst(port_id, 0, bufs, 32);

        for (int i = 0; i < n; ++i) {
            rte_mbuf* m = bufs[i];
            std::cout << "Got a packet" << '\n';

            std::byte* p = rte_pktmbuf_mtod(m, std::byte*);
            uint16_t len = m->pkt_len;

            //parser.parse(src, len, ob_bm_handler);
            rte_pktmbuf_free(m);
        }
    }

    #ifndef PERF
    export_latency_distribution_csv(ob_bm_handler, outdir + "parsing_and_order_book_latency_distribution.csv");
    export_prices_csv(ob_bm_handler.prices, outdir);
    #endif

    run_noise = false;
    noise.join();

    return 0;
}
