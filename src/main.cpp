#include <array>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "parser_v1.hpp"
#include "parser_v2.hpp"

template<typename MsgType, size_t N>
struct CounterHandler {
    void handle(const MsgType& msg) {
        messages_num++;
        counts[static_cast<unsigned char>(msg.type)]++;
    }

    void reset() {
        messages_num = 0;
        counts.fill(0);
    }

    std::array<uint64_t, N> counts{};
    uint64_t messages_num = 0;
};

template<typename Parser, typename Handler>
static void run_one(
    const char* name,
    Parser& parser,
    Handler& handler,
    const std::byte* src,
    size_t len
) {
    using clock = std::chrono::high_resolution_clock;

    handler.reset();

    auto start = clock::now();
    parser.parse(src, len, handler);
    auto end = clock::now();

    double seconds =
        std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

    uint64_t msgs = handler.messages_num;
    double throughput = msgs / seconds;
    double avg_ns = (seconds * 1e9) / double(msgs);

    std::cout << "=== " << name << " ===\n";
    std::cout << "Messages:        " << msgs << "\n";
    std::cout << "Seconds:         " << seconds << "\n";
    std::cout << "Throughput:      " << throughput << " msg/s\n";
    std::cout << "Average latency: " << avg_ns << " ns/msg\n\n";
}

template<typename Handler>
static void dump_counts(const Handler& handler) {
    for (size_t i = 0; i < handler.counts.size(); ++i) {
        if (handler.counts[i] == 0) continue;
        std::cout << char(i) << ' ' << handler.counts[i] << '\n';
    }
}

int main() {
    std::ifstream file("../data/01302019.NASDAQ_ITCH50", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::vector<std::byte> src_buf;
    src_buf.resize(10 * 4096);

    file.read(reinterpret_cast<char*>(src_buf.data()), src_buf.size());
    size_t bytes_read = size_t(file.gcount());

    if (bytes_read < 3) {
        std::cerr << "File read too small\n";
        return 1;
    }

    const std::byte* src = src_buf.data();
    size_t len = bytes_read;

    ITCHv1::ItchParser parser_v1;
    ITCHv2::ItchParser parser_v2;

    CounterHandler<ITCHv1::Message, 256> h1;
    CounterHandler<ITCHv2::Message, 256> h2;

    run_one("ITCH v1", parser_v1, h1, src, len);
    run_one("ITCH v2", parser_v2, h2, src, len);

    std::cout << "\n=== Per-type counts (v2) ===\n";
    dump_counts(h2);
    return 0;
}

