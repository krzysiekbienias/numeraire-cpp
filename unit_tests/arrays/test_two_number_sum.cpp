#include <gtest/gtest.h>
#include "arrays/two_number_sum.hpp"



TEST(TwoNumberSumSortedTest, FoundPair) {
    std::vector<int> nums = {3, 5, -4, 8, 11, 1, -1, 6};
    int target = 10;

    auto result = twoNumberSum(nums, target);

    // We expect [−1, 11], [1, 9], etc., depending on sort
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0] + result[1], target);
}

TEST(TwoNumberSumSortedTest, NoPairFound) {
    std::vector<int> nums = {1, 2, 3, 4};
    int target = 100;

    auto result = twoNumberSum(nums, target);

    EXPECT_TRUE(result.empty());
}

TEST(TwoNumberSumSortedTest, DuplicatesAllowed) {
    std::vector<int> nums = {5, 5, 3};
    int target = 10;

    auto result = twoNumberSum(nums, target);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0] + result[1], target);
}
