#include "cpp/examples/thread_pool.h"

#include "gtest/gtest.h"

#include <vector>

TEST(ThreadPoolTest, ThreadPool) {
  examples::ThreadPool* instance = examples::ThreadPool::Instance();
  std::vector<int> arr = {1, 2, 3, 4, 5};
  // 避免返回太快导致 arr 析构， func 里访问到无效数组而 core，采用 =
  auto func = [=]() {
    int sum = 0;
    for (auto i : arr) {
      sum += i;
    }
    ASSERT_EQ(sum, 15);
  };
  instance->Run(func);
}