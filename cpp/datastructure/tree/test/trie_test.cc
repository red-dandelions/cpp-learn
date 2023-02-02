#include "cpp/datastructure/tree/trie.h"

#include "gtest/gtest.h"

TEST(TrieTest, solve) {
  tree::Trie t;
  t.insert("red-dandelion");
  ASSERT_TRUE(t.start_with("red"));
  ASSERT_FALSE(t.query("red"));
  ASSERT_TRUE(t.query("red-dandelion"));
}