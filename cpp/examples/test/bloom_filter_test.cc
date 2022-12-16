#include "cpp/examples/bloom_filter.h"

#include "gtest/gtest.h"

#include <string>
#include <vector>

TEST(BloomFilter, BloomFilter) {
  // Insert, Exists
  {
    double fpp = 0.001;
    uint64_t elem_size = 1000;
    examples::BloomFilter cbf(fpp, elem_size);
    ASSERT_EQ(cbf.hash_function_number(), 10);
    ASSERT_GE(cbf.bucket_size(), 14378);

    std::vector<std::string> string_keys = {"key1", "key2"};
    std::vector<uint64_t> keys = {1ul, 2ul};

    int sz1 = sizeof(uint64_t);
    ASSERT_FALSE(cbf.Exists(nullptr, 0));

    cbf.Insert(&keys[0], sz1);
    ASSERT_TRUE(cbf.Exists(&keys[0], sz1));
    // may be false
    EXPECT_FALSE(cbf.Exists(&keys[1], sz1));

    ASSERT_FALSE(cbf.Exists(string_keys[0].c_str(), string_keys[0].size()));
    cbf.Insert(string_keys[1].c_str(), string_keys[1].size());
    ASSERT_TRUE(cbf.Exists(string_keys[1].c_str(), string_keys[1].size()));
  }

  // InsertedLookup
  {
    double fpp = 0.001;
    uint64_t elem_size = 1000;
    examples::BloomFilter cbf(fpp, elem_size);
    uint64_t key = 123;
    bool res = cbf.InsertedLookup(&key, sizeof(key));
    ASSERT_TRUE(res);
  }
}

TEST(BloomFilter, CountingBloomFilter) {
  // Insert, Exists
  {
    double fpp = 0.001;
    uint64_t elem_size = 1000;
    examples::CountingBloomFilter<uint8_t> cbf(fpp, elem_size);
    ASSERT_EQ(cbf.hash_function_number(), 10);
    ASSERT_GE(cbf.bucket_size(), 14378);

    std::vector<uint64_t> keys = {1ul, 2ul, 3ul, 4ul, 1ul, 2ul, 2ul};
    int sz1 = sizeof(uint64_t);
    int sz2 = sz1 * 2;
    ASSERT_TRUE(cbf.Exists(nullptr, 0, 0));
    ASSERT_FALSE(cbf.Exists(nullptr, 0, 1));
    ASSERT_TRUE(cbf.Exists(&keys[0], sz1, 0));
    cbf.Insert(&keys[0], sz1);
    ASSERT_TRUE(cbf.Exists(&keys[0], sz1, 1));
    // must be false
    ASSERT_FALSE(cbf.Exists(&keys[0], sz1, 2));
    // may be false
    EXPECT_FALSE(cbf.Exists(&keys[1], sz1, 1));
    // may be false
    EXPECT_FALSE(cbf.Exists(&keys[0], sz2, 1));
    cbf.Insert(&keys[1], sz1);
    cbf.Insert(&keys[2], sz1);
    cbf.Insert(&keys[3], sz1);
    cbf.Insert(&keys[4], sz1);
    ASSERT_TRUE(cbf.Exists(&keys[4], sz1, 2));
    cbf.Insert(&keys[5], sz1);
    cbf.Insert(&keys[6], sz1);
    ASSERT_TRUE(cbf.Exists(&keys[1], sz1, 3));
  }

  // InsertedLookup
  {
    double fpp = 0.001;
    uint64_t elem_size = 1000;
    examples::CountingBloomFilter<uint8_t> cbf(fpp, elem_size);
    uint64_t key = 123;
    for (int i = 0; i < 300; ++i) {
      bool res = cbf.InsertedLookup(&key, sizeof(key), 255);
      if (i >= 255) ASSERT_TRUE(res);
    }
    ASSERT_FALSE(cbf.InsertedLookup(&key, sizeof(key), 256));
  }
}