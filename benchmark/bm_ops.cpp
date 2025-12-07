#include <benchmark/benchmark.h>

static void BM_bswap32(benchmark::State& state) {
    uint32_t t = 87760787;

    for (auto _ : state) {
        benchmark::DoNotOptimize(t);
        uint32_t res = 0;
        res = __builtin_bswap32(t);
        benchmark::DoNotOptimize(res);
        benchmark::ClobberMemory();
    }
}

static void BM_bswap64(benchmark::State& state) {
    uint64_t t = 87760787;

    for (auto _ : state) {
        benchmark::DoNotOptimize(t);
        uint64_t res = 0;
        res = __builtin_bswap64(t);
        benchmark::DoNotOptimize(res);
        benchmark::ClobberMemory();
    }
}

BENCHMARK(BM_bswap32);
BENCHMARK(BM_bswap64);
