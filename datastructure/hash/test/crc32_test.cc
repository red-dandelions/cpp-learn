#include "datastructure/hash/crc32.h"

#include "gtest/gtest.h"

#include <iomanip>
#include <sstream>

std::string uint32_to_string(uint32_t res) {
  std::ostringstream oss;
  oss << std::setw(8) << std::setfill('0') << std::hex << res;
  return oss.str();
}

TEST(CRC32TEST, crc32) {
  uint32_t res;
  // default seed
  {
    datastructure::Crc32 hash_fn(0);
    std::string key0("");
    std::string val0("00000000");
    std::string key1("this is a test line");
    std::string val1("7d69e53b");
    std::string key2("This is a test line");
    std::string val2("5bed932f");
    hash_fn(key0.c_str(), key0.size(), &res);
    ASSERT_EQ(uint32_to_string(res), val0);
    hash_fn(key1.c_str(), key1.size(), &res);
    ASSERT_EQ(uint32_to_string(res), val1);
    hash_fn(key2.c_str(), key2.size(), &res);
    ASSERT_EQ(uint32_to_string(res), val2);
  }

  // use seed
  {
    datastructure::Crc32 hash_fn(7621u);
    std::string key0("");
    std::string val0("00001dc5");
    std::string key1("this is a test line");
    std::string val1("49f2cf0a");
    hash_fn(key0.c_str(), key0.size(), &res);
    ASSERT_EQ(uint32_to_string(res), val0);
    hash_fn(key1.c_str(), key1.size(), &res);
    ASSERT_EQ(uint32_to_string(res), val1);
  }
}