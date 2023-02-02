#include "thread_pool.h"

namespace reactor {

// 获取线程池单例
ThreadPool* ThreadPool::Instance() {
  static ThreadPool tp(std::thread::hardware_concurrency());
  return &tp;
}

void ThreadPool::Loop() {
  while (true) {
    std::function<void()> task;
    {
      // 只有一个线程先拿到锁，先拿到锁的线程则执行任务
      std::unique_lock<std::mutex> lock(queue_mutex_);
      // 可以避免虚假唤醒
      condition_.wait(lock, [this] {
        return this->stop_ || !this->tasks_.empty();
      });  // 返回 false 则阻塞，返回 true 则取消阻塞
      if (stop_ && tasks_.empty()) return;
      task = std::move(tasks_.front());
      tasks_.pop();
    }
    task();
  }
}

ThreadPool::ThreadPool(size_t threads) : stop_(false) {
  for (size_t i = 0; i < threads; ++i) {
    workers_.emplace_back([this] { Loop(); });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  // 唤醒所有线程
  condition_.notify_all();
  for (std::thread& worker : workers_) {
    worker.join();
  }
}

void ThreadPool::Run(const std::function<void()>& func) {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    tasks_.emplace(func);
  }
  // 唤醒一个线程执行任务
  condition_.notify_one();
}

}  // namespace reactor