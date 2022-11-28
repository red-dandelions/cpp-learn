#include "templates/simd_sum.h"

#include "gtest/gtest.h"
#include <vector>

TEST(SIMDSum, sum) {
  const int Size = 100000;
  std::vector<int> a(Size, 1), b(Size, 2), c(Size, 3);
  templates::Sum(a, b);
  for (size_t i = 0; i < Size; ++i) {
    ASSERT_EQ(a[i], c[i]);
  }

  std::vector<float> a(Size, 1.5), b(Size, 2.5), c(Size, 4);
  templates::Sum(a, b);
  for (size_t i = 0; i < Size; ++i) {
    ASSERT_EQ(a[i], c[i]);
  }
}