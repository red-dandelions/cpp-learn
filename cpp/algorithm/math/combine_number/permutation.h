#pragma once

#include <cinttypes>

namespace math {

// 排列组合数求解， 时间和空间复杂度： O(m)
int64_t permutation(int n, int m) {
  if (n == m || m == 0) return 1;
  return permutation(n, m - 1) * (n - m + 1) / m;
}

}  // namespace math