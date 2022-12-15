#pragma once

#include <cstdint>

namespace datastructure {
namespace hash {

class MurmurHash {
 public:
  MurmurHash(uint32_t seed) : seed_(seed) {}
  void operator()(const void* key, int len, void* out);

 private:
  uint32_t seed_;
};
}  // namespace hash
}  // namespace datastructure