#include "cpp/examples/spin_lock.h"

#include "gtest/gtest.h"

#include <mutex>
#include <thread>

int num = 0;
examples::SpinLock spin_mutex;

void func() {
  for (size_t i = 0; i < 10000000; ++i) {
    std::lock_guard<examples::SpinLock> lock(spin_mutex);
    num++;
  }
}

TEST(SpinLockTest, SpinLock) {
  std::thread th1(func);
  std::thread th2(func);
  th1.join();
  th2.join();
  ASSERT_EQ(num, 20000000);
}