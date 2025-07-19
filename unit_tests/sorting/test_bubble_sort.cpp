#include "gtest/gtest.h"
#include "sorting/bubble_sort.hpp"

TEST(BubbleSort, AlreadySorted) {
    std::vector<int> input{1, 2, 3, 4, 5};
    std::vector<int> expected{1, 2, 3, 4, 5};
    EXPECT_EQ(bubbleSort(input), expected);
}

TEST(BubbleSort, ReverseSorted) {
    std::vector<int> input{5, 4, 3, 2, 1};
    std::vector<int> expected{1, 2, 3, 4, 5};
    EXPECT_EQ(bubbleSort(input), expected);
}

TEST(BubbleSort, RandomOrder) {
    std::vector<int> input{4, 2, 5, 1, 3};
    std::vector<int> expected{1, 2, 3, 4, 5};
    EXPECT_EQ(bubbleSort(input), expected);
}

TEST(BubbleSort, ContainsDuplicates) {
    std::vector<int> input{3, 1, 2, 2, 3};
    std::vector<int> expected{1, 2, 2, 3, 3};
    EXPECT_EQ(bubbleSort(input), expected);
}

TEST(BubbleSort, SingleElement) {
    std::vector<int> input{42};
    std::vector<int> expected{42};
    EXPECT_EQ(bubbleSort(input), expected);
}

TEST(BubbleSort, EmptyVector) {
    std::vector<int> input{};
    std::vector<int> expected{};
    EXPECT_EQ(bubbleSort(input), expected);
}

TEST(BubbleSort, NegativeNumbers) {
    std::vector<int> input{-2, -5, -1, -3};
    std::vector<int> expected{-5, -3, -2, -1};
    EXPECT_EQ(bubbleSort(input), expected);
}
