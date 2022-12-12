#include "datastructure/hash/md5.h"

#include "gtest/gtest.h"

#include <iomanip>
#include <sstream>

std::string uint128_to_string(uint64_t res[2]) {
  std::ostringstream oss;
  oss << std::setw(16) << std::setfill('0') << std::hex << res[0];
  oss << std::setw(16) << std::setfill('0') << std::hex << res[1];
  return oss.str();
}

std::string uint32_to_string(uint32_t res) {
  std::ostringstream oss;
  oss << std::setw(8) << std::setfill('0') << std::hex << res;
  return oss.str();
}

TEST(MurmurHashTest, MurmurHash) {
  // 128
  {
    uint64_t res[2];
    datastructure::Md5 hash_fn;
    std::string key0("");
    std::string val0("04b2008fd98c1dd47e42f8ec980980e9");
    std::string key1("this is a test line");
    std::string val1("50b25d53d674bed4eb1af9f3c3ba2a25");
    hash_fn(key0.c_str(), key0.size(), res);
    ASSERT_EQ(uint128_to_string(res), val0);
    hash_fn(key1.c_str(), key1.size(), res);
    ASSERT_EQ(uint128_to_string(res), val1);
  }
  // 32
  {
    uint32_t res;
    datastructure::Md5 hash_fn;
    std::string key0("");
    std::string val0("d98c1dd4");
    std::string key1("this is a test line");
    std::string val1("d674bed4");
    hash_fn(key0.c_str(), key0.size(), &res, datastructure::Md5::Type::md5_32);
    ASSERT_EQ(uint32_to_string(res), val0);
    hash_fn(key1.c_str(), key1.size(), &res);
    ASSERT_EQ(uint32_to_string(res), val1);
  }
}