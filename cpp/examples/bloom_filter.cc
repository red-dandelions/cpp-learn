#include "bloom_filter.h"

namespace examples {

BloomFilter::BloomFilter(double fpp, uint64_t element_size)
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
  buf_.resize(bucket_size_, false);
}

void BloomFilter::Insert(const void* key, int len) {
  if (key == nullptr) return;
  uint64_t murmurhash_res[2];  // murmurhash 生成 128 位结果
  for (auto&& fn : hash_functions_) {
    fn(key, len, murmurhash_res);
    uint64_t index = murmurhash_res[1] % bucket_size_;
    buf_[index] = true;
  }
}

bool BloomFilter::Exists(const void* key, int len) const {
  if (key == nullptr) return false;
  uint64_t murmurhash_res[2];  // murmurhash 生成 128 位结果
  for (auto&& fn : hash_functions_) {
    fn(key, len, murmurhash_res);
    uint64_t index = murmurhash_res[1] % bucket_size_;
    if (buf_[index] == false) return false;
  }
  return true;
}

bool BloomFilter::InsertedLookup(const void* key, int len) {
  Insert(key, len);
  return Exists(key, len);
}

}  // namespace examples