#pragma once

#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>

namespace examples {

template <typename Task>
class BlockingQueue {
 public:
  BlockingQueue(size_t capacity) : capacity_(capacity) {}
  virtual ~BlockingQueue() {}

  void Enqueue(const Task& task);
  void ForceEnqueue(const Task& task);
  bool TryEnqueue(const Task& task, uint32_t timewait);
  void EnqueueFront(const Task& task);

  Task Dequeue();
  bool TryDequeue(Task* task, uint32_t timewait);

  size_t Size() {
    std::unique_lock<std::mutex> lock(mutex_);
    return tasks_.size();
  }

  bool Full(uint32_t timewait) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (tasks_.size() < capacity_) return false;
    cv_full_.wait_for(lock, std::chrono::milliseconds(timewait));
    return tasks_.size() >= capacity_;
  }

  bool Empty() {
    std::unique_lock<std::mutex> lock(mutex_);
    return tasks_.size() == 0;
  }

  void Clear() {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.clear();
  }

  void ClearAndDelete(void (*deleter)(Task)) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (tasks_.empty()) {
      Task task = tasks_.front();
      tasks_.pop_front();
      deleter(task);
    }
  }

 protected:
  size_t capacity_;
  std::mutex mutex_;
  std::condition_variable cv_empty_;
  std::condition_variable cv_full_;
  std::deque<Task> tasks_;
};

template <typename Task>
void BlockingQueue<Task>::Enqueue(const Task& task) {
  std::unique_lock<std::mutex> lock(mutex_);
  cv_full_.wait(lock, [this] { return tasks_.size() < capacity_; });
  tasks_.emplace_back(task);
  cv_empty_.notify_all();
}

template <typename Task>
void BlockingQueue<Task>::ForceEnqueue(const Task& task) {
  std::unique_lock<std::mutex> lock(mutex_);
  tasks_.emplace_back(task);
  cv_empty_.notify_all();
}

template <typename Task>
bool BlockingQueue<Task>::TryEnqueue(const Task& task, uint32_t timewait) {
  std::unique_lock<std::mutex> lock(mutex_);
  cv_full_.wait_for(lock, std::chrono::milliseconds(timewait),
                    [this] { return tasks_.size() < capacity_; });
  if (tasks_.size() >= capacity_) return false;
  tasks_.emplace_back(task);
  cv_empty_.notify_all();
  return true;
}

template <typename Task>
void BlockingQueue<Task>::EnqueueFront(const Task& task) {
  std::unique_lock<std::mutex> lock(mutex_);
  cv_full_.wait(lock, [this] { return tasks_.size() < capacity_; });
  tasks_.emplace_front(task);
  cv_empty_.notify_all();
}

template <typename Task>
Task BlockingQueue<Task>::Dequeue() {
  std::unique_lock<std::mutex> lock(mutex_);
  cv_empty_.wait(lock, [this] { return !tasks_.empty(); });
  Task task = tasks_.front();
  tasks_.pop_front();
  cv_full_.notify_all();
  return task;
}

template <typename Task>
bool BlockingQueue<Task>::TryDequeue(Task* task, uint32_t timewait) {
  std::unique_lock<std::mutex> lock(mutex_);
  cv_empty_.wait_for(lock, std::chrono::milliseconds(timewait), [this] { return !tasks_.empty(); });
  if (tasks_.empty()) return false;
  *task = tasks_.front();
  tasks_.pop_front();
  cv_full_.notify_all();
  return true;
}

}  // namespace examples