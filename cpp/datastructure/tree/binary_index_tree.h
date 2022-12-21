#pragma once

#include <cinttypes>
#include <vector>

namespace tree {

// 树状数组
// arr_[i] 表示 [i - lowbit(i) + 1, i] 范围和
// 初始化数组为 n + 1
// 将数值放入 1 .. n 中
template <typename T>
class BinaryIndexTree {
 public:
  BinaryIndexTree<T>(int64_t n) : size_(n) {
    diff_.resize(size_ + 1, 0);
    idiff_.resize(size_ + 1, 0);
  }

  // O(n) 建树，vec 为初始化数组
  void init(std::vector<T>& vec) {
    size_ = vec.size();
    diff_.resize(size_ + 1, 0);
    idiff_.resize(size_ + 1, 0);

    for (int64_t i = 1; i <= size_; ++i) {
      diff_[i] += (i == 1) ? vec[i - 1] : vec[i - 1] - vec[i - 2];
      idiff_[i] += (i == 1) ? i * vec[i - 1] : i * (vec[i - 1] - vec[i - 2]);

      int64_t j = i + __lowbit(i);
      if (j <= size_) {
        diff_[j] += diff_[i];
        idiff_[j] += idiff_[i];
      }
    }
  }

  // 单点修改
  // O(log n)
  void update(int64_t i, T val) { range_update(i, i, val); }

  // 单点查询
  int64_t query(int64_t i) {
    int64_t result = 0;
    while (i) {
      result += diff_[i];
      i -= __lowbit(i);
    }
    return result;
  }

  // 区间修改
  // O(log n)
  void range_update(int64_t left, int64_t right, T val) {
    auto __update = [&](int i, T val) {
      if (i <= 0) i = 1;
      T ival = i * val;
      while (i <= size_) {
        diff_[i] += val;
        idiff_[i] += ival;
        i += __lowbit(i);
      }
    };
    __update(left, val);
    __update(right + 1, -val);
  }

  // 前缀和
  // O(log n)
  int64_t prefix_sum(int i) { return range_sum(1, i); }

  // 区间求和
  // O(log n)
  int64_t range_sum(int64_t l, int64_t r) {
    auto _query = [&](int64_t i, std::vector<T>& arr) -> int64_t {
      int64_t result = 0;
      while (i) {
        result += arr[i];
        i -= __lowbit(i);
      }
      return result;
    };
    return (r + 1) * _query(r, diff_) - _query(r, idiff_) - l * _query(l - 1, diff_) +
           _query(l - 1, idiff_);
  }

 private:
  // 返回 arr_[i] 管理多少个数组元素的和
  // i 的二进制表示中，最低位的 1 的位置。
  int64_t __lowbit(int64_t i) { return i & -i; }

  int64_t size_;
  std::vector<T> diff_;
  std::vector<T> idiff_;
};

}  // namespace tree