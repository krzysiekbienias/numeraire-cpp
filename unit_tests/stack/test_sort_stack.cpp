#include <gtest/gtest.h>
#include "stack/sort_stack.hpp"


TEST(SortStackTest, EmptyStack) {
    std::vector<int> stack{};
    sortStack(stack);
    EXPECT_TRUE(stack.empty());
}

TEST(SortStackTest, SingleElement) {
    std::vector<int> stack{5};
    sortStack(stack);
    EXPECT_EQ(stack, std::vector<int>({5}));
}

TEST(SortStackTest, AlreadySortedStack) {
    std::vector<int> stack{1, 2, 3, 4, 5};
    sortStack(stack);
    EXPECT_EQ(stack, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(SortStackTest, ReverseSortedStack) {
    std::vector<int> stack{5, 4, 3, 2, 1};
    sortStack(stack);
    EXPECT_EQ(stack, std::vector<int>({1, 2, 3, 4, 5}));
}

TEST(SortStackTest, RandomStackSample) {
    std::vector<int> stack{3, 1, 4, 6, 2, 8};
    sortStack(stack);
    EXPECT_EQ(stack, std::vector<int>({1, 2, 3, 4, 6, 8}));
}

TEST(SortStackTest, WithDuplicates) {
    std::vector<int> stack{3, 1, 3, 2, 1};
    sortStack(stack);
    EXPECT_EQ(stack, std::vector<int>({1, 1, 2, 3, 3}));
}
