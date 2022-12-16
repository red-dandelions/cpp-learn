#include "cpp/examples/simd_sum.h"

#include "gtest/gtest.h"

#include <iostream>
#include <vector>

TEST(SIMDSum, normal) {
  const int Size = 10;
  {
    std::vector<int> a(Size, 1), b(Size, 2), c(Size, 3);
    examples::Sum(a, b);
    for (size_t i = 0; i < Size; ++i) {
      ASSERT_EQ(a[i], c[i]);
    }
  }
}

TEST(SIMDSum, simd) {
  const int Size = 10;
  {
    std::vector<float> a(Size, 1.5), b(Size, 2.5), c(Size, 4);
    examples::Sum(a, b);
    for (size_t i = 0; i < Size; ++i) {
      ASSERT_EQ(a[i], c[i]);
    }
  }
}