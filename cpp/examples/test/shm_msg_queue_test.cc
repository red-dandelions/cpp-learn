#include "cpp/examples/shm_msg_queue.h"

#include "glog/logging.h"
#include "gtest/gtest.h"

#include <thread>

// 测试 BoundedConcurrentQueue
TEST(SharedMemoryMessageQueue, msg_queue_multi_thread) {
  auto q = examples::SharedMemoryMessageQueue();
  q.init(nullptr);

  const int32_t msgs[] = {1, 2, 3, 4};

  std::thread t([&q, &msgs]() {
    ASSERT_TRUE(q.enqueue(msgs, sizeof(msgs)));  //
  });

  uint32_t out_chunk_id = 0;
  const int32_t* out_msg = nullptr;
  size_t out_msg_size = 0;
  ASSERT_TRUE(q.dequeue(&out_chunk_id, reinterpret_cast<const void**>(&out_msg), &out_msg_size,
                        INT64_MAX) == examples::SharedMemoryMessageQueue::Status::ok);

  ASSERT_EQ(out_msg_size, sizeof(msgs));
  for (size_t i = 0; i < 4; ++i) {
    ASSERT_EQ(msgs[i], out_msg[i]) << "i: " << i;
  }

  q.release_chunk(out_chunk_id);

  t.join();
}

TEST(SharedMemoryMessageQueue, allocate) {
  examples::SharedMemoryMessageQueue shm_msgq;
  examples::SharedMemoryMessageQueue::Options options = {
      .capacity = 3,
      .buffer_size = 1,
  };
  CHECK(shm_msgq.init(&options));

  for (int32_t i = 0; i < 10; ++i) {
    const size_t msg_size = 1024 * 1024 - 1;
    uint32_t msg_id = 0;
    void* msg_addr = nullptr;
    CHECK(shm_msgq.allocate(msg_size, true, &msg_id, &msg_addr));

    shm_msgq.release_chunk(msg_id);
  }
}

TEST(SharedMemoryMessageQueue, close) {
  {
    examples::SharedMemoryMessageQueue q;
    q.init(nullptr);

    q.set_preducer_count(1);
    q.close();

    ASSERT_TRUE(q.dequeue(nullptr, nullptr, nullptr, INT64_MAX) !=
                examples::SharedMemoryMessageQueue::Status::ok);
  }

  {
    examples::SharedMemoryMessageQueue q;
    q.init(nullptr);
    q.set_preducer_count(1);

    std::thread thr([&q]() {
      uint32_t out_chunk_id = 0;
      size_t msg_size = 0;
      const void* msg_data = nullptr;
      auto ret = q.dequeue(&out_chunk_id, &msg_data, &msg_size, INT64_MAX) ==
                 examples::SharedMemoryMessageQueue::Status::ok;
      ASSERT_TRUE(ret == false);

      q.signal_preducing_complete();
    });

    q.close();

    thr.join();
  }
}

TEST(SharedMemoryMessageQueue, multi_thread_2) {
  constexpr int32_t kWorkerCount = 100;
  constexpr int32_t kPerWorkerSize = 100;

  examples::SharedMemoryMessageQueue q;
  q.init(nullptr);
  q.set_preducer_count(kWorkerCount);

  std::vector<std::thread> workers;
  for (int32_t i = 0; i < kWorkerCount; ++i) {
    workers.push_back(std::thread([&q]() {
      char buf[2];
      for (int32_t j = 0; j < kPerWorkerSize; ++j) {
        CHECK(q.enqueue(buf, sizeof(buf)) == true);
      }

      q.signal_preducing_complete();
    }));
  }

  int32_t count = 0;
  while (true) {
    uint32_t msg_id = -1;
    size_t msg_size = 0;
    const void* msg_data = nullptr;
    auto ret = q.dequeue(&msg_id, &msg_data, &msg_size, INT64_MAX);
    if (ret == examples::SharedMemoryMessageQueue::Status::closed) {
      break;
    }
    count++;
    q.release_chunk(msg_id);
  }
  ASSERT_EQ(count, kWorkerCount * kPerWorkerSize);

  for (auto& w : workers) w.join();
}

TEST(SharedMemoryMessageQueue, msg_queue_multi_process) {
  examples::SharedMemoryMessageQueue q;
  q.init(nullptr);
  q.set_preducer_count(1);

  const int32_t msgs[] = {1, 2, 3, 4};

  int p = fork();
  if (p == 0) {
    ASSERT_TRUE(q.enqueue(msgs, sizeof(msgs)));
    q.signal_preducing_complete();
  } else {
    uint32_t out_chunk_id = 0;
    const int32_t* out_msg = nullptr;
    size_t out_msg_size = 0;
    ASSERT_TRUE(q.dequeue(&out_chunk_id, reinterpret_cast<const void**>(&out_msg), &out_msg_size,
                          INT64_MAX) == examples::SharedMemoryMessageQueue::Status::ok);

    ASSERT_EQ(out_msg_size, sizeof(msgs));
    for (size_t i = 0; i < 4; ++i) {
      ASSERT_EQ(msgs[i], out_msg[i]) << "i: " << i;
    }

    q.release_chunk(out_chunk_id);

    // 等待子进程退出
    waitpid(p, nullptr, 0);
  }
}

TEST(SharedMemoryMessageQueue, multi_process_test_2) {
  constexpr int32_t kWorkerCount = 100;
  const int32_t kPerMessageSize = 32;
  constexpr int32_t kPerWorkerMessageCount = 5;

  examples::SharedMemoryMessageQueue q;
  examples::SharedMemoryMessageQueue::Options options = {
      .capacity = kWorkerCount + 3,
      .buffer_size = kPerMessageSize * (kWorkerCount + 3),
  };
  q.init(&options);
  q.set_preducer_count(kWorkerCount);

  std::vector<pid_t> worker_pids;
  for (int32_t i = 0; i < kWorkerCount; ++i) {
    int pid = fork();
    if (pid == 0) {
      for (int32_t j = 0; j < kPerWorkerMessageCount; ++j) {
        uint32_t msg_id = 0;
        void* msg_addr = nullptr;
        q.allocate(kPerMessageSize * 1024 * 1024, true, &msg_id, &msg_addr);
        q.enqueue(msg_id);
      }

      q.signal_preducing_complete();

      _exit(EXIT_SUCCESS);
    }

    else {
      worker_pids.push_back(pid);
    }
  }

  // 获取消息
  int recv_count = 0;
  uint32_t msg_id = 0;
  const void* msg_addr = nullptr;
  size_t msg_size = 0;
  while (q.dequeue(&msg_id, &msg_addr, &msg_size, INT64_MAX) ==
         examples::SharedMemoryMessageQueue::Status::ok) {
    q.release_chunk(msg_id);
    recv_count++;
  }

  for (auto& pid : worker_pids) {
    int32_t status;
    waitpid(pid, &status, 0);
  }

  CHECK_EQ(recv_count, kPerWorkerMessageCount * kWorkerCount);
}