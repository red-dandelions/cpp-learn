#include "gtest/gtest.h"

int add(int a, int b) { return a + b; }

TEST(main, add) { EXPECT_EQ(add(3, 4), 7); }