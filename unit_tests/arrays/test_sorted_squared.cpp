#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include "arrays/sorted_squared.hpp"

TEST(SortedSquaredFasterTest, HandlesEmptyVector) {
    std::vector<int> input = {};
    std::vector<int> expected = {};
    EXPECT_EQ(sortedSquaredFaster(input), expected);
}

TEST(SortedSquaredFasterTest, HandlesAllPositiveValues) {
    std::vector<int> input = {1, 2, 3, 4};
    std::vector<int> expected = {1, 4, 9, 16};
    EXPECT_EQ(sortedSquaredFaster(input), expected);
}

TEST(SortedSquaredFasterTest, HandlesAllNegativeValues) {
    std::vector<int> input = {-4, -3, -2, -1};
    std::vector<int> expected = {1, 4, 9, 16};
    EXPECT_EQ(sortedSquaredFaster(input), expected);
}

TEST(SortedSquaredFasterTest, HandlesMixedValues) {
    std::vector<int> input = {-7, -3, 1, 4, 8};
    std::vector<int> expected = {1, 9, 16, 49, 64};
    EXPECT_EQ(sortedSquaredFaster(input), expected);
}

TEST(SortedSquaredFasterTest, HandlesZerosAndDuplicates) {
    std::vector<int> input = {-2, -2, 0, 2, 3};
    std::vector<int> expected = {0, 4, 4, 4, 9};
    EXPECT_EQ(sortedSquaredFaster(input), expected);
}

TEST(SortedSquaredFasterTest, HandlesSingleElement) {
    std::vector<int> input = {-5};
    std::vector<int> expected = {25};
    EXPECT_EQ(sortedSquaredFaster(input), expected);
}
