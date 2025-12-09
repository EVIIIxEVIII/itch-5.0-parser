#include <benchmark/benchmark.h>
#include <fstream>
#include <vector>
#include <cstddef>
#include <iostream>

#include "parser.hpp"

static const std::vector<std::byte> &first_chunk()
{
    static std::vector<std::byte> buf = [] {
        std::ifstream file("../data/01302019.NASDAQ_ITCH50",
                           std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open ITCH file");
        }

        std::vector<std::byte> tmp(4096);
        file.read(reinterpret_cast<char *>(tmp.data()),
                  static_cast<std::streamsize>(tmp.size()));
        std::streamsize got = file.gcount();
        if (got <= 0) {
            throw std::runtime_error("Failed to read first chunk");
        }
        tmp.resize(static_cast<std::size_t>(got));
        return tmp;
    }();
    return buf;
}

static void BM_ParseMsg(benchmark::State& state) {
    static const std::vector<std::byte> buf = first_chunk();
    ItchParser parser;

    const std::byte* src = buf.data();
    MessageType type;

    for (auto _ : state) {
        auto msg = parser.parseMsg(src);
        benchmark::DoNotOptimize(msg);
        type = msg.type;
        benchmark::DoNotOptimize(type);
    }

    std::cout << char(type) << '\n';
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_ParseMsg);
