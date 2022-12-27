#include "cpp/algorithm/lib/sort/sort.h"

#include "gtest/gtest.h"

#include <random>

#include "algorithm"

std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

std::vector<int> generate_v() {
  std::vector<int> arr = v;

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(arr.begin(), arr.end(), g);

  return arr;
}

TEST(SortTest, BubbleSort) {
  std::vector<int> arr = generate_v();

  sort::bubble_sort<int>(arr);

  for (int32_t i = 0; i < arr.size(); ++i) {
    ASSERT_EQ(v[i], arr[i]);
  }
}

TEST(SortTest, InsertSort) {
  std::vector<int> arr = generate_v();

  sort::insert_sort<int>(arr);

  for (int32_t i = 0; i < arr.size(); ++i) {
    ASSERT_EQ(v[i], arr[i]);
  }
}

TEST(SortTest, SelectSort) {
  std::vector<int> arr = generate_v();

  sort::select_sort<int>(arr);

  for (int32_t i = 0; i < arr.size(); ++i) {
    ASSERT_EQ(v[i], arr[i]);
  }
}

TEST(SortTest, ShellSort) {
  std::vector<int> arr = generate_v();

  sort::shell_sort<int>(arr);

  for (int32_t i = 0; i < arr.size(); ++i) {
    ASSERT_EQ(v[i], arr[i]);
  }
}

TEST(SortTest, MergeSort) {
  std::vector<int> arr = generate_v();

  sort::merge_sort<int>(arr, 0, arr.size());

  for (int32_t i = 0; i < arr.size(); ++i) {
    ASSERT_EQ(v[i], arr[i]);
  }
}

TEST(SortTest, HeapSort) {
  std::vector<int> arr = generate_v();

  sort::heap_sort<int>(arr);

  for (int32_t i = 0; i < arr.size(); ++i) {
    ASSERT_EQ(v[i], arr[i]);
  }
}

TEST(SortTest, QuickSort) {
  std::vector<int> arr = generate_v();

  sort::quick_sort<int>(arr, 0, arr.size() - 1);

  for (int32_t i = 0; i < arr.size(); ++i) {
    ASSERT_EQ(v[i], arr[i]);
  }
}

TEST(SortTest, BucketSort) {
  // TODO
}

TEST(SortTest, RadixSort) {
  // TODO
}

TEST(SortTest, CountSort) {
  // TODO
}