#pragma once

#include <string>
#include <unordered_set>

namespace hash {

class StringHash {
 public:
  // 滚动字符哈希，size <= s.size()
  int operator()(std::string s, int32_t size, int32_t start = 0) {
    // 计算 size 长度的 s 的哈希值
    const uint64_t b = 97755331;
    uint64_t h = 0, p = 1;
    for (int32_t i = start; i < size; ++i) {
      h = h * b + s[i];
      p *= b;
    }

    // 存入集合
    std::unordered_set<uint64_t> hs{h};

    // 从下标 idx = size 开始一次计算长度为 size 的 s 的哈希值，存入集合中
    // 如果已经存在了，返回 idx;
    // 配合二分查找，可以用于计算重复子串的问题
    for (int32_t idx = start + size; idx < s.size(); ++idx) {
      h = h * b - p * s[idx - size] + s[idx];
      if (hs.find(h) != hs.end()) {
        return idx;
      }
      hs.insert(h);
    }
    return 0;
  }
};

}  // namespace hash