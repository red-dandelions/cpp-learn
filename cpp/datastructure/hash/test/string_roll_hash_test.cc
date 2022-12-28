/**
 * 1044. 最长重复子串
 *
 * 给你一个字符串 s ，考虑其所有 重复子串 ：即 s 的（连续）子串，在 s 中出现 2
 * 次或更多次。这些出现之间可能存在重叠。 返回 任意一个 可能具有最长长度的重复子串。如果 s
 * 不含重复子串，那么答案为 "" 。
 */
#include "cpp/datastructure/hash/string_roll_hash.h"

#include "gtest/gtest.h"

TEST(StringRollHashTest, LongestDupSubstring) {
  std::string s = "banana";
  auto solve = [](std::string& s) -> std::string {
    hash::StringHash strHash;
    int left = 0, right = s.size() - 1;
    int ans = 0;
    while (left < right) {
      int mid = (left + right + 1) >> 1, ret;
      (ret = strHash(s, mid)) ? left = mid : right = mid - 1;
      ret ? ans = ret : ans;
    }
    return s.substr(ans - left + 1, left);
  };
  std::string res = solve(s);
  ASSERT_EQ(res, "ana");
}