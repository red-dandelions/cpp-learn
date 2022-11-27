#include "examples/gtest/add.h"

#include "gtest/gtest.h"

TEST(AddTest, add) {
    int res = examples::gtest::add(3,4);

    ASSERT_EQ(res, 7);
}