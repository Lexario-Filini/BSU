#include <gtest/gtest.h>
#include "array_utils.h"
#include <vector>

TEST(ComputeAverageTests, PositiveIntegers) {
    std::vector<int> v{ 1, 2, 3, 4, 5 };
    double avg = compute_average(v);
    EXPECT_DOUBLE_EQ(avg, 3.0);
}

TEST(ComputeAverageTests, MixedSignIntegers) {
    std::vector<int> v{ -2, 0, 3, 7 };
    double avg = compute_average(v);
    EXPECT_DOUBLE_EQ(avg, 2.0);
}

TEST(ComputeAverageTests, SingleElement) {
    std::vector<int> v{ 42 };
    double avg = compute_average(v);
    EXPECT_DOUBLE_EQ(avg, 42.0);
}

TEST(ComputeAverageTests, RepeatingValues) {
    std::vector<int> v{ 5, 5, 5, 5 };
    double avg = compute_average(v);
    EXPECT_DOUBLE_EQ(avg, 5.0);
}

TEST(ComputeAverageMoreTests, TwoElementsFraction) {
    std::vector<int> v{ 1, 2 };
    double avg = compute_average(v);
    EXPECT_DOUBLE_EQ(avg, 1.5);
}

TEST(ComputeAverageMoreTests, LargeValues) {
    std::vector<int> v{ 1000000000, 1000000000, 1000000000 };
    double avg = compute_average(v);
    EXPECT_DOUBLE_EQ(avg, 1000000000.0);
}

TEST(ReplaceMinMaxTests, ReplaceDistinctMinMax) {
    std::vector<int> v{ 1, 4, 2, 3 };
    replace_min_max_with_value(v, 10);
    std::vector<int> expected{ 10, 10, 2, 3 };
    EXPECT_EQ(v, expected);
}

TEST(ReplaceMinMaxTests, ReplaceWhenMultipleMinOrMax) {
    std::vector<int> v{ 7, 1, 7, 3, 1 };
    replace_min_max_with_value(v, -5);
    std::vector<int> expected{ -5, -5, -5, 3, -5 };
    EXPECT_EQ(v, expected);
}

TEST(ReplaceMinMaxTests, AllElementsEqual) {
    std::vector<int> v{ 2, 2, 2 };
    replace_min_max_with_value(v, 99);
    std::vector<int> expected{ 99, 99, 99 };
    EXPECT_EQ(v, expected);
}

TEST(ReplaceMinMaxTests, NegativeValues) {
    std::vector<int> v{ -10, -3, -7, -3 };
    replace_min_max_with_value(v, 0);
    std::vector<int> expected{ 0, 0, -7, 0 };
    EXPECT_EQ(v, expected);
}

TEST(ReplaceMinMaxMoreTests, PreserveMiddleElements) {
    std::vector<int> v{ 0, 5, 10, 5, 0 };
    replace_min_max_with_value(v, 7);
    std::vector<int> expected{ 7, 5, 7, 5, 7 };
    EXPECT_EQ(v, expected);
}

TEST(ReplaceMinMaxMoreTests, TwoElements) {
    std::vector<int> v{ 8, 3 };
    replace_min_max_with_value(v, -1);
    std::vector<int> expected{ -1, -1 };
    EXPECT_EQ(v, expected);
}

TEST(ReplaceMinMaxMoreTests, MixedNegativeAndZero) {
    std::vector<int> v{ -2, 0, -2, -1, 0 };
    replace_min_max_with_value(v, 42);
    std::vector<int> expected{ 42, 42, 42, -1, 42 };
    EXPECT_EQ(v, expected);
}

TEST(EdgeCaseTests, EmptyVectorComputeAverage) {
    std::vector<int> v;
    double avg = compute_average(v);
    EXPECT_DOUBLE_EQ(avg, 0.0);
}

TEST(EdgeCaseTests, EmptyVectorReplaceMinMax) {
    std::vector<int> v;
    replace_min_max_with_value(v, 5);
    EXPECT_TRUE(v.empty());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
