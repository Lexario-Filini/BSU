#include <gtest/gtest.h>
#include "factorials.h"
#include "dedup.h"
#include "linked_list.h"
#include "errors.h"

TEST(FactorialsTest, FirstFive) {
    auto v = core::first_n_factorials(5);
    std::vector<std::uint64_t> expected = { 1, 2, 6, 24, 120 };
    EXPECT_EQ(v, expected);
}

TEST(FactorialsTest, ZeroThrows) {
    EXPECT_THROW(core::first_n_factorials(0), core::ValidationError);
}

TEST(FactorialsTest, OverflowThrows) {
    EXPECT_THROW(core::first_n_factorials(30), core::OverflowError);
}

TEST(FactorialsTest, OneReturnsOne) {
    auto v = core::first_n_factorials(1);
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], 1u);
}

TEST(FactorialsTest, LargeButSafe) {
    auto v = core::first_n_factorials(20);
    EXPECT_EQ(v.back(), 2432902008176640000ULL); 
}

TEST(DedupTest, RemovesDuplicatesPreservesOrder) {
    std::vector<int> data = { 1,2,2,3,1,4,4,5 };
    auto result = core::dedup_preserve_order(data);
    std::vector<int> expected = { 1,2,3,4,5 };
    EXPECT_EQ(result, expected);
}

TEST(DedupTest, EmptyInput) {
    std::vector<int> data;
    auto result = core::dedup_preserve_order(data);
    EXPECT_TRUE(result.empty());
}

TEST(DedupTest, AllUnique) {
    std::vector<int> data = { 10,20,30 };
    auto result = core::dedup_preserve_order(data);
    EXPECT_EQ(result, data);
}

TEST(DedupTest, AllSame) {
    std::vector<int> data = { 7,7,7,7 };
    auto result = core::dedup_preserve_order(data);
    std::vector<int> expected = { 7 };
    EXPECT_EQ(result, expected);
}

TEST(LinkedListTest, ReverseStrings) {
    std::vector<std::string> words = { "a","b","c" };
    auto lst = core::List<std::string>::from_vector(words);
    lst.reverse();
    auto result = lst.to_vector();
    std::vector<std::string> expected = { "c","b","a" };
    EXPECT_EQ(result, expected);
}

TEST(LinkedListTest, ReverseSingleElement) {
    std::vector<int> nums = { 42 };
    auto lst = core::List<int>::from_vector(nums);
    lst.reverse();
    auto result = lst.to_vector();
    EXPECT_EQ(result, nums);
}

TEST(LinkedListTest, ReverseEmpty) {
    core::List<int> lst;
    lst.reverse();
    EXPECT_TRUE(lst.to_vector().empty());
}

TEST(LinkedListTest, ReversePalindrome) {
    std::vector<int> nums = { 1,2,3,2,1 };
    auto lst = core::List<int>::from_vector(nums);
    lst.reverse();
    auto result = lst.to_vector();
    std::vector<int> expected = { 1,2,3,2,1 };
    EXPECT_EQ(result, expected);
}

TEST(LinkedListTest, ReverseLongerList) {
    std::vector<int> nums = { 1,2,3,4,5,6,7,8,9,10 };
    auto lst = core::List<int>::from_vector(nums);
    lst.reverse();
    auto result = lst.to_vector();
    std::vector<int> expected = { 10,9,8,7,6,5,4,3,2,1 };
    EXPECT_EQ(result, expected);
}
