#include "benchmark/benchmark.h"
#include "cpp/datastructure/stack/stack.h"

#include <random>
#include <stack>

void run_stack(int32_t count, int32_t size) {
  stack::Stack<uint32_t> s;
  std::mt19937 gen(0);  // 随机数引擎
  std::uniform_int_distribution<uint32_t> dist(
      0, std::numeric_limits<uint32_t>::max());  // 正太分布随机数
  for (int32_t i = 0; i < count; ++i) {
    for (int32_t j = 0; j < size; ++j) {
      s.push(dist(gen));
    }
  }
  for (int32_t i = 0; i < count; ++i) {
    for (int32_t j = 0; j < size; ++j) {
      s.pop();
    }
  }
}

void run_std_stack(uint32_t count, int32_t size) {
  std::stack<uint32_t> s;
  std::mt19937 gen(0);  // 随机数引擎
  std::uniform_int_distribution<uint32_t> dist(
      0, std::numeric_limits<uint32_t>::max());  // 正太分布随机数
  for (int32_t i = 0; i < count; ++i) {
    for (int32_t j = 0; j < size; ++j) {
      s.push(dist(gen));
    }
  }
  for (int32_t i = 0; i < count; ++i) {
    for (int32_t j = 0; j < size; ++j) {
      s.pop();
    }
  }
}

static void BM_stack(benchmark::State& state) {
  for (auto _ : state) {
    run_stack(4, 1024 * 1024);
  }
}

static void BM_std_stack(benchmark::State& state) {
  for (auto _ : state) {
    run_std_stack(4, 1024 * 1024);
  }
}

BENCHMARK(BM_stack);
BENCHMARK(BM_std_stack);

BENCHMARK_MAIN();