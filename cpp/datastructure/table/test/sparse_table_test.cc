/**
 * 给定数组 input 和 q，数组 q 每次询问input区间 [q[i][0], q[i][1]] 内数字的最大值。
 */
#include "cpp/datastructure/table/sparse_table.h"
#include "gtest/gtest.h"

TEST(SparseTableTest, solve) {
  std::vector<int> input = {9, 3, 1, 7, 5, 6, 0, 8};
  std::vector<std::vector<int>> q = {{1, 6},{1,5},{2,7},{2,6},{1,8},{4,8},{3,7},{1,8}};
  std::vector<int> output = {9, 9, 7, 7, 9, 8, 7, 9};

  table::SparseTable<int> st(input);
  std::vector<int> res;
  for (int i = 0; i < q.size(); ++i) {
    res.push_back(st.query(q[i][0] - 1, q[i][1] - 1));
  }
  for (int i = 0; i < output.size(); ++i) {
    ASSERT_EQ(res[i], output[i]) << i;
  }
}