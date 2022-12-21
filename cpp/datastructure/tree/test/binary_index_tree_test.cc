#include "cpp/datastructure/tree/binary_index_tree.h"

#include "gtest/gtest.h"

#include <algorithm>

using tree::BinaryIndexTree;

/**
 * 在数组中的两个数字，如果前面一个数字大于后面的数字，则这两个数字组成一个逆序对。输入一个数组，求出这个数组中的逆序对的总数。
 */
TEST(BinaryIndexTreeTest, reverse_pair) {
  std::vector<int> input{7, 5, 6, 5, 4};
  auto func = [](std::vector<int>& input) -> int {
    if (input.empty()) {
      return 0;
    }
    int n = input.size();
    auto num = input;
    std::sort(input.begin(), input.end());
    for (int& v : num) {
      v = std::lower_bound(input.begin(), input.end(), v) - input.begin() + 1;
    }

    BinaryIndexTree<int> t(n);
    int ans = 0;
    // 从尾到头计算严格顺序对
    for (int i = n - 1; i >= 0; --i) {
      ans += t.prefix_sum(num[i] - 1);
      t.update(num[i], 1);
    }
    return ans;
  };
  ASSERT_EQ(func(input), 8);
}