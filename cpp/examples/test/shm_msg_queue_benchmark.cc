#include "benchmark/benchmark.h"
#include "cpp/examples/shm_msg_queue.h"
#include "glog/logging.h"

#include <chrono>
#include <sys/wait.h>
#include <unistd.h>

void run_shm_msg_queue(int32_t worker_count, int32_t msg_count, int32_t per_msg_size) {
  examples::SharedMemoryMessageQueue queue("benchmark");
  queue.init(nullptr);

  std::vector<pid_t> workers;
  queue.set_preducer_count(worker_count);

  for (int32_t i = 0; i < worker_count; ++i) {
    auto p = fork();
    if (p != 0) {
      workers.push_back(p);
    } else {
      std::vector<uint8_t> buff(per_msg_size, 0);
      for (int32_t j = 0; j < msg_count; ++j) {
        CHECK(queue.enqueue(buff.data(), buff.size()) == true);
      }

      queue.signal_preducing_complete();

      exit(0);  // 退出进程
    }
  }

  auto tp0 = std::chrono::steady_clock::now();

  int recv_msg_count = 0;
  int64_t byte_count = 0;
  while (true) {
    uint32_t msg_id = 0;
    const void* msg_addr = nullptr;
    size_t msg_size = 0;
    auto status = queue.dequeue(&msg_id, &msg_addr, &msg_size, INT64_MAX);
    if (status != examples::SharedMemoryMessageQueue::Status::ok) {
      break;
    }
    recv_msg_count++;

    queue.release_chunk(msg_id);

    if (recv_msg_count % 1000 == 0) LOG(INFO) << "recv_msg_count: " << recv_msg_count;
    byte_count += msg_size;
  }
  auto tp1 = std::chrono::steady_clock::now();

  LOG(INFO) << "throughput: " << double(byte_count) / double((tp1 - tp0).count()) * 1000 << " Mb/s";

  // 等待子进程退出
  for (auto w : workers) {
    int32_t status;
    waitpid(w, &status, 0);
  }
  LOG(INFO) << "all workers quit";
}

static void BM_shm_msg_queue(benchmark::State& state) {
  for (auto _ : state) {
    run_shm_msg_queue(100, 10, 1024 * 1024);
  }
}

BENCHMARK(BM_shm_msg_queue);

BENCHMARK_MAIN();