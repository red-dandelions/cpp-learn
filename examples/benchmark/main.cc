#include "benchmark/benchmark.h"

void add(int a, int b) { int c = a + b; }

static void benchmark_add(benchmark::State& state) {
  for (auto _ : state) {
    add(3, 4);
  }
}

BENCHMARK(benchmark_add);

BENCHMARK_MAIN();