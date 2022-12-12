#pragma once

#include <cstdint>

namespace datastructure {

class Crc32 {
 public:
  Crc32(uint32_t seed) : seed_(seed) {}
  void operator()(const void* key, int len, void* out);

 private:
  uint32_t seed_;
};

}  // namespace datastructure