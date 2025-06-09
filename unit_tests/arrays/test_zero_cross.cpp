#include <gtest/gtest.h>
#include "arrays/zero_cross.hpp"

TEST(ZeroCrossTest, BasicTest) {
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 0, 6},
        {7, 8, 9}
    };

    std::vector<std::vector<int>> expected = {
        {1, 0, 3},
        {0, 0, 0},
        {7, 0, 9}
    };

    zeroCross(matrix);

    EXPECT_EQ(matrix, expected);
}

TEST(ZeroCrossTest, AllZerosTest) {
    std::vector<std::vector<int>> matrix = {
        {0, 0},
        {0, 0}
    };

    std::vector<std::vector<int>> expected = {
        {0, 0},
        {0, 0}
    };

    zeroCross(matrix);

    EXPECT_EQ(matrix, expected);
}

TEST(ZeroCrossTest, NoZerosTest) {
    std::vector<std::vector<int>> matrix = {
        {1, 2},
        {3, 4}
    };

    std::vector<std::vector<int>> expected = {
        {1, 2},
        {3, 4}
    };

    zeroCross(matrix);

    EXPECT_EQ(matrix, expected);
}

TEST(ZeroCrossTest, EmptyMatrixTest) {
    std::vector<std::vector<int>> matrix;

    std::vector<std::vector<int>> expected;

    zeroCross(matrix);

    EXPECT_EQ(matrix, expected);
}
