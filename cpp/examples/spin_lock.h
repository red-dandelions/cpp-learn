#pragma once

#include <atomic>

namespace examples {

class SpinLock {
 public:
  SpinLock() = default;
  SpinLock(const SpinLock&) = delete;
  SpinLock& operator=(const SpinLock&) = delete;
  ~SpinLock() {}

  void lock() noexcept {
    bool expected = false;
    // lock_ == expected  ->  lock_ = true;
    // else expected = lock_;
    while (!lock_.compare_exchange_strong(expected, true, std::memory_order_acquire)) {
      expected = false;
    }
  }

  void unlock() noexcept { lock_.store(false, std::memory_order_release); }

 private:
  std::atomic<bool> lock_ = {false};
};

}  // namespace examples
