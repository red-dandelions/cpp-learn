/**
 * bloom filter
 * m = - n * ln(p) / ln(2) ^ 2
 * k = m / n * ln(2)
 */
#pragma once

#include "cpp/datastructure/hash/murmurhash.h"

#include <cmath>
#include <functional>
#include <memory>
#include <mutex>
#include <random>
#include <set>
#include <vector>

namespace examples {

class BloomFilterBase {
 public:
  virtual void Insert(const void* key, int len) = 0;
  virtual bool Exists(const void* key, int len, uint32_t max_count = 1u) const = 0;
  virtual bool InsertedLookup(const void* key, int len, uint32_t max_count = 1u) = 0;
};

class BloomFilter {
 public:
  using Hasher = std::function<void(const void*, int, void*)>;  // hash 函数
  BloomFilter(double fpp = 0.01, uint64_t element_size = 1000000000);
  ~BloomFilter() {}

  virtual void Insert(const void* key, int len);
  virtual bool Exists(const void* key, int len) const;
  virtual bool InsertedLookup(const void* key, int len);

  double false_positive_probability() { return false_positive_probability_; }
  uint64_t element_size() { return element_size_; }
  uint64_t bucket_size() { return bucket_size_; }
  uint32_t hash_function_number() { return hash_function_number_; }

 private:
  BloomFilter(const BloomFilter&) = delete;
  BloomFilter(const BloomFilter&&) = delete;
  BloomFilter& operator=(const BloomFilter&) = delete;
  BloomFilter& operator=(const BloomFilter&&) = delete;

  double false_positive_probability_;
  uint64_t element_size_;
  uint64_t bucket_size_;
  uint32_t hash_function_number_;
  std::vector<Hasher> hash_functions_;
  std::vector<bool> buf_;
};

/**
 * element_size_: 数据规模， 对应公式中的 n
 * false_positive_probability_: 误判率， 对应公式中的 p
 * bucket_size_: 所需要的二进位数， 对应公式中的 m
 * hash_function_number_: hash 函数个数， 对应公式中的 k
 * hash_functions_: hash 函数数组 (murmurhash)
 * buf_: 二进制数组
 */
template <typename T>
class CountingBloomFilter : public BloomFilterBase {
 public:
  using Hasher = std::function<void(const void*, int, void*)>;  // hash 函数

  CountingBloomFilter(double fpp = 0.001, uint64_t element_size = 1000000000);
  ~CountingBloomFilter() {}

  virtual void Insert(const void* key, int len) override;
  virtual bool Exists(const void* key, int len, uint32_t max_count = 1u) const override;
  virtual bool InsertedLookup(const void* key, int len, uint32_t max_count = 1u) override;

  double false_positive_probability() { return false_positive_probability_; }
  uint64_t element_size() { return element_size_; }
  uint64_t bucket_size() { return bucket_size_; }
  uint64_t hash_function_number() { return hash_function_number_; }

 private:
  CountingBloomFilter(const CountingBloomFilter&) = delete;
  CountingBloomFilter(const CountingBloomFilter&&) = delete;
  CountingBloomFilter& operator=(const CountingBloomFilter&) = delete;
  CountingBloomFilter& operator=(const CountingBloomFilter&&) = delete;

  double false_positive_probability_;
  uint64_t element_size_;
  uint64_t bucket_size_;
  uint64_t hash_function_number_;
  std::vector<Hasher> hash_functions_;
  std::vector<T> buf_;
};

template class CountingBloomFilter<uint8_t>;
template class CountingBloomFilter<uint16_t>;

template <typename T>
CountingBloomFilter<T>::CountingBloomFilter(double fpp, uint64_t element_size)
    : false_positive_probability_(fpp), element_size_(element_size) {
  bucket_size_ = static_cast<uint64_t>(
      std::ceil(std::abs(std::log(fpp) * element_size / std::pow(std::log(2), 2))));
  hash_function_number_ = static_cast<uint32_t>(std::ceil(std::abs(std::log2(fpp))));  // 换底公式

  std::set<uint32_t> seeds;
  // 使用随机数生成 seed
  std::random_device rd;   // 硬件生成随机种子
  std::mt19937 gen(rd());  // 利用种子生成随机数引擎
  std::uniform_int_distribution<uint32_t> dist(
      0, std::numeric_limits<uint32_t>::max());  // 正太分布随机数
  while (seeds.size() != hash_function_number_) {
    seeds.insert(dist(gen));
  }

  // 生成 hash 函数
  for (const auto& seed : seeds) {
    hash_functions_.emplace_back(datastructure::hash::MurmurHash(seed));
  }
  // 初始化 bit 数组
  buf_.resize(bucket_size_, T());
}

template <typename T>
void CountingBloomFilter<T>::Insert(const void* key, int len) {
  if (key == nullptr) return;
  uint64_t murmurhash_res[2];
  for (auto&& fn : hash_functions_) {
    fn(key, len, murmurhash_res);
    uint64_t index = murmurhash_res[1] % bucket_size_;
    if (buf_[index] != std::numeric_limits<T>::max()) {
      buf_[index] += 1;
    }
  }
}

template <typename T>
bool CountingBloomFilter<T>::Exists(const void* key, int len, uint32_t max_count) const {
  if (max_count == 0) return true;
  if (key == nullptr) return false;
  uint64_t murmurhash_res[2];
  for (auto&& fn : hash_functions_) {
    fn(key, len, murmurhash_res);
    uint64_t index = murmurhash_res[1] % bucket_size_;
    if (buf_[index] < max_count) return false;
  }
  return true;
}

template <typename T>
bool CountingBloomFilter<T>::InsertedLookup(const void* key, int len, uint32_t max_count) {
  Insert(key, len);
  return Exists(key, len, max_count);
}

}  // namespace examples