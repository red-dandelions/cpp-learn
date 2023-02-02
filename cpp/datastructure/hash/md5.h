#pragma once

#include <cstdint>
#include <cstring>

namespace datastructure {
namespace hash {

class Md5 {
 public:
  enum class Type : int32_t {
    md5_32,
    md5_128,
  };

  Md5() {}
  void operator()(const void* key, int len, void* out, enum Type type = Type::md5_128);
};

}  // namespace hash
}  // namespace datastructure