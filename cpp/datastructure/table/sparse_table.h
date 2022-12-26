#pragma once

#include <cmath>
#include <functional>
#include <vector>

namespace table {

template <typename T>
class SparseTable {
 public:
  using Func = std::function<T(const T &, const T &)>;

  SparseTable(const std::vector<T> &v, Func func = _max) {
    op_ = func;
    int len = v.size(), l1 = std::ceil(log2(len)) + 1;
    sparse_table_.assign(len, std::vector(l1, 0));
    for (int32_t i = 0; i < len; ++i) {
      sparse_table_[i][0] = v[i];
    }
    for (int32_t j = 1; j < l1; ++j) {
      int32_t pj = (1 << (j - 1));
      for (int32_t i = 0; i + pj < len; ++i) {
        sparse_table_[i][j] =
            op_(sparse_table_[i][j - 1], sparse_table_[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  T query(int32_t l, int32_t r) {
    int32_t lt = r - l + 1;
    int32_t q = std::ceil(std::log2(lt)) - 1;
    return op_(sparse_table_[l][q], sparse_table_[r - (1 << q) + 1][q]);
  }

 private:
  static T _max(const T &a, const T &b) { return std::max<T>(a, b); }

  Func op_;
  std::vector<std::vector<T>> sparse_table_;
};

}  // namespace table