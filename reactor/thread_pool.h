#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace reactor {

class ThreadPool {
 public:
  ThreadPool(size_t threads);
  ~ThreadPool();

  static ThreadPool* Instance();

  void Run(const std::function<void()>& func);

 private:
  void Loop();

  std::vector<std::thread> workers_;         // 工作线程
  std::queue<std::function<void()>> tasks_;  // 任务队列

  std::mutex queue_mutex_;             // 互斥量
  std::condition_variable condition_;  // 条件变量
  bool stop_;                          // 停止标志
};

}  // namespace reactor