/**
 * 给定一个整数数组 temperatures ，表示每天的温度，返回一个数组 answer ，
 * 其中 answer[i] 是指对于第 i 天，下一个更高温度出现在几天后。
 * 如果气温在这之后都不会升高，请在该位置用 0 来代替。
 *
 * 输入: temperatures = [73,74,75,71,69,72,76,73]
 * 输出: [1,1,4,2,1,1,0,0]
 */
#include "gtest/gtest.h"

#include <stack>
#include <vector>

TEST(MonotonicStack, solve) {
  std::vector<int> input = {73, 74, 75, 71, 69, 72, 76, 73};
  std::vector<int> output = {1, 1, 4, 2, 1, 1, 0, 0};
  std::stack<int> s;

  auto solve = [&](std::vector<int>& in) -> std::vector<int> {
    std::vector<int> out(in.size(), 0);
    std::stack<std::pair<int, int>> s;
    for (int i = in.size() - 1; i >= 0; --i) {
      while (!s.empty() && s.top().first < input[i]) {
        s.pop();
      }
      if (!s.empty()) out[i] = s.top().second - i;
      s.push(std::make_pair(input[i], i));
    }
    return out;
  };

  auto res = solve(input);

  for (int i = 0; i < output.size(); ++i) {
    ASSERT_EQ(output[i], res[i]);
  }
}