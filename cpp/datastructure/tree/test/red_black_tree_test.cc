#include "cpp/datastructure/tree/red_black_tree.h"

#include "gtest/gtest.h"

#include <iostream>
#include <queue>
#include <vector>

TEST(RedBlackTreeTest, red_black_tree_test) {
  tree::RBTree<int, int> t;
  for (int i = 1; i <= 11; ++i) {
    t.insert(i, i);
  }

  auto node = t.find(7);
  ASSERT_TRUE(node != nullptr);
  ASSERT_TRUE(t.erase(7));
  ASSERT_FALSE(t.erase(7));
  ASSERT_TRUE(t.find(7) == nullptr);
}