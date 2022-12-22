#include "benchmark/benchmark.h"
#include "cpp/datastructure/tree/heap.h"

#include <queue>
#include <random>
void run_heap(int32_t count, int32_t size) {
  tree::MaxHeap<uint32_t> mh;
  std::mt19937 gen(0);  // 随机数引擎
  std::uniform_int_distribution<uint32_t> dist(
      0, std::numeric_limits<uint32_t>::max());  // 正太分布随机数
  for (int32_t i = 0; i < count; ++i) {
    for (int32_t j = 0; j < size; ++j) {
      mh.push(dist(gen));
    }
  }
  for (int32_t i = 0; i < count; ++i) {
    for (int32_t j = 0; j < size; ++j) {
      mh.pop();
    }
  }
}

void run_std_heap(uint32_t count, int32_t size) {
  std::priority_queue<uint32_t> mh;
  std::mt19937 gen(0);  // 随机数引擎
  std::uniform_int_distribution<uint32_t> dist(
      0, std::numeric_limits<uint32_t>::max());  // 正太分布随机数
  for (int32_t i = 0; i < count; ++i) {
    for (int32_t j = 0; j < size; ++j) {
      mh.push(dist(gen));
    }
  }
  for (int32_t i = 0; i < count; ++i) {
    for (int32_t j = 0; j < size; ++j) {
      mh.pop();
    }
  }
}

static void BM_heap(benchmark::State& state) {
  for (auto _ : state) {
    run_heap(4, 1024 * 1024);
  }
}

static void BM_std_heap(benchmark::State& state) {
  for (auto _ : state) {
    run_std_heap(4, 1024 * 1024);
  }
}

BENCHMARK(BM_heap);
BENCHMARK(BM_std_heap);

BENCHMARK_MAIN();