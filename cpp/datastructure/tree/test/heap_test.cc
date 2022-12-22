#include "cpp/datastructure/tree/heap.h"

#include "gtest/gtest.h"

TEST(HeapTest, MinHeap) {
  std::vector<int32_t> data = {1, 2, 4, 7, 3, 9, 5, 7, 1, 5, 1, 4};
  tree::MinHeap<int32_t> mh;
  for (auto v : data) {
    mh.push(v);
  }
  std::vector<int32_t> res;
  while (!mh.empty()) {
    res.push_back(mh.top());
    mh.pop();
  }
  std::sort(data.begin(), data.end());
  for (size_t i = 0; i < data.size(); ++i) {
    ASSERT_EQ(res[i], data[i]);
  }
}

TEST(HeapTest, MaxHeap) {
  std::vector<int32_t> data = {1, 2, 4, 7, 3, 9, 5, 7, 1, 5, 1, 4};
  tree::MaxHeap<int32_t> mh;
  for (auto v : data) {
    mh.push(v);
  }
  std::vector<int32_t> res;
  while (!mh.empty()) {
    res.push_back(mh.top());
    mh.pop();
  }
  std::sort(data.begin(), data.end(), [](int32_t a, int32_t b) { return a > b; });
  for (size_t i = 0; i < data.size(); ++i) {
    ASSERT_EQ(res[i], data[i]);
  }
}