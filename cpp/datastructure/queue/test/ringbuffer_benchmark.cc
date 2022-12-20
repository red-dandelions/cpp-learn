#include "benchmark/benchmark.h"
#include "cpp/datastructure/queue/ringbuffer.h"
#include "glog/logging.h"

#include <chrono>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace datastructure::queue;

void run_ringbuffer(int32_t count, int32_t size) {
  RingBuffer ringbuffer;
  size_t init_size = 128 * (1 << 20);  // 128 M
  ringbuffer.init(init_size);

  std::thread th([&]() {
    RingBuffer::Node node;
    node.size = size;
    node.data = std::vector<uint8_t>(size, 1);
    for (int32_t i = 0; i < count; ++i) {
      ringbuffer.push(node);
    }
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  RingBuffer::Node node;
  int64_t byte_count = 0;
  auto tp0 = std::chrono::high_resolution_clock::now();
  while (!ringbuffer.empty()) {
    node = ringbuffer.pop();
    byte_count += node.size;
  }
  auto tp1 = std::chrono::high_resolution_clock::now();
  LOG(INFO) << "byte_count: " << byte_count << " total_size: " << count * size;
  LOG(INFO) << "throughput: " << double(byte_count) / double((tp1 - tp0).count()) * 1000 << " Mb/s";

  th.join();
}

static void BM_ringbuffer(benchmark::State& state) {
  for (auto _ : state) {
    run_ringbuffer(512, 1024 * 1024);
  }
}

BENCHMARK(BM_ringbuffer);

BENCHMARK_MAIN();