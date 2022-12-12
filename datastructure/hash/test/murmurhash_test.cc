#include "datastructure/hash/murmurhash.h"

#include "gtest/gtest.h"

#include <iomanip>
#include <sstream>

std::string uint128_to_string(uint64_t res[2]) {
  std::ostringstream oss;
  oss << std::setw(16) << std::setfill('0') << std::hex << res[0];
  oss << std::setw(16) << std::setfill('0') << std::hex << res[1];
  return oss.str();
}

TEST(MurmurHashTest, MurmurHash) {
  uint64_t res[2];
  // default seed
  {
    datastructure::MurmurHash hash_fn(0);
    std::string key0("");
    std::string val0("00000000000000000000000000000000");
    std::string key1("this is a test line");
    std::string val1("097eec3aaefa7332e0d14cf2d59d26c3");
    hash_fn(key0.c_str(), key0.size(), res);
    ASSERT_EQ(uint128_to_string(res), val0);
    hash_fn(key1.c_str(), key1.size(), res);
    ASSERT_EQ(uint128_to_string(res), val1);
  }

  // use seed
  {
    datastructure::MurmurHash hash_fn(7621u);
    std::string key0("");
    std::string val0("2087cc3f33e14ed78d8fdd6d054f1c2c");
    std::string key1("this is a test line");
    std::string val1("b3c8e4d2b3806183bc117d2cd9cf9a3e");
    hash_fn(key0.c_str(), key0.size(), res);
    ASSERT_EQ(uint128_to_string(res), val0);
    hash_fn(key1.c_str(), key1.size(), res);
    ASSERT_EQ(uint128_to_string(res), val1);
  }
}