#pragma once

#include <cinttypes>
#include <numeric>
#include <vector>

namespace search {
// 并查集
class UF {
 public:
  UF(int n) : count_(n), father_(n), size_(n, 1) {
    // 将[0, n - 1]填充到__father
    std::iota(father_.begin(), father_.end(), 0);
  }

  // 返回并查集个数
  int32_t count() { return count_; }

  // 递归寻找根结点, 返回根结点
  int32_t FindAndSet(int32_t v) {
    return father_[v] == v ? v : father_[v] = FindAndSet(father_[v]);
  }

  // 同一个集合合并，减少并查集个数
  void Unit(int x, int y) {
    if (size_[x] < size_[y]) {
      std::swap(x, y);
    }
    father_[y] = x;
    size_[x] += size_[y];
    --count_;
  }

  // x, y不在同一个集合，返回true，否则返回false
  bool FindAndUnit(int x, int y) {
    int a = FindAndSet(x);
    int b = FindAndSet(y);
    if (a != b) {
      Unit(a, b);
      return true;
    }
    return false;
  }

 private:
  int32_t count_;  // 存储有多少个并查集
  std::vector<int32_t> father_;
  std::vector<int32_t> size_;  //__size存储每个并查集的大小。
};

}  // namespace search