//
// Created by psmolnik on 12/20/2020.
//

#include "gtest/gtest.h"
#include "hashes.hpp"
#include "hash_map_chained.hpp"
#include "hash_open_addressing.hpp"
#include "hash_map_cuckoo.hpp"

template <typename T>
class HashMapChainedTest : public testing::Test {};
using test_types = testing::Types<int, double, long double>;

TYPED_TEST_CASE(HashMapChainedTest, test_types);
TYPED_TEST(HashMapChainedTest, test_types)
{
    auto hash_map = HashMapChained<TypeParam, std::string, CustomHash<TypeParam> >{10};
    size_t hash_map_exp_size = 16;
    EXPECT_TRUE(hash_map.size() == hash_map_exp_size);

    hash_map.insert(1, "first");
    hash_map.insert(2, "second");

    EXPECT_EQ(hash_map.find(1), "first");

    EXPECT_NO_THROW(hash_map.erase(1));
    EXPECT_ANY_THROW(hash_map.erase(1));

    EXPECT_NO_THROW(hash_map.find(2));
    EXPECT_ANY_THROW(hash_map.find(1));
}

template <typename Hash>
class HashMapOpenAddressingTest : public testing::Test {};

using probing_types = testing::Types<CustomHashLinear<int>,
                                     CustomHashQuadratic<int>,
                                     CustomHashDouble<int> >;

TYPED_TEST_CASE(HashMapOpenAddressingTest, probing_types);
TYPED_TEST(HashMapOpenAddressingTest, test_probing)
{
    auto hash_map = HashMapOpenAddressing<int, std::string, TypeParam>{10};
    size_t hash_map_exp_size = 16;
    EXPECT_TRUE(hash_map.size() == hash_map_exp_size);

    hash_map.insert(1, "first");
    hash_map.insert(2, "second");

    EXPECT_EQ(hash_map.find(1), "first");

    EXPECT_NO_THROW(hash_map.erase(1));
    EXPECT_ANY_THROW(hash_map.erase(1));

    EXPECT_NO_THROW(hash_map.find(2));
    EXPECT_ANY_THROW(hash_map.find(1));
}

class CuckooHashTest : public testing::Test {};

TEST(CuckooHashTest, cuckoo) {
    auto hash_map = HashMapCuckoo<int, std::string, CustomHash<int>>{10};
    size_t hash_map_exp_size = 16;
    EXPECT_TRUE(hash_map.size() == hash_map_exp_size);

    hash_map.insert(1, "first", true);
    hash_map.insert(2, "second", true);

    EXPECT_EQ(hash_map.find(1), "first");

    EXPECT_NO_THROW(hash_map.erase(1));
    EXPECT_ANY_THROW(hash_map.erase(1));

    EXPECT_NO_THROW(hash_map.find(2));
    EXPECT_ANY_THROW(hash_map.find(1));
}