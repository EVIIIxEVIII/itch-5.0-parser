#include <benchmark/benchmark.h>
#include "utils.hpp"
#include "parser_v2.hpp"

struct HandlerV2 {
    uint64_t messages = 0;

    void handle(const ITCHv2::Message& msg) {
        messages++;
    }
};

static void BM_Parse_v2(benchmark::State& state) {
    static std::vector<std::byte> buf = load_chunk(8 * 4096);

    ITCHv2::ItchParser parser;
    uint64_t total_messages = 0;

    for (auto _ : state) {
        HandlerV2 handler{};
        parser.parse(buf.data(), buf.size(), handler);
        total_messages += handler.messages;
        benchmark::DoNotOptimize(handler.messages);
    }

    state.counters["msg/s"] =
        benchmark::Counter(total_messages, benchmark::Counter::kIsRate);

    state.counters["ns/msg"] =
    benchmark::Counter(double(total_messages) / 1e9,
                       benchmark::Counter::kIsRate | benchmark::Counter::kInvert);
}

BENCHMARK(BM_Parse_v2);
