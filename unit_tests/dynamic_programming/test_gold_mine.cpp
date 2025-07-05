
#include "gtest/gtest.h"
#include "dynamic_programming/gold_mine.hpp"

TEST(GoldMineTest, BasicSquareGrid) {
    std::vector<std::vector<int>> mine = {
        {1, 3, 1},
        {2, 1, 4},
        {0, 6, 4}
    };
    EXPECT_EQ(goldMine(mine), 13); // Path: 3 → 4 → 6
}

TEST(GoldMineTest, SingleRow) {
    std::vector<std::vector<int>> mine = {
        {10, 20, 30}
    };
    EXPECT_EQ(goldMine(mine), 30); // Just the max in row 0
}

TEST(GoldMineTest, SingleColumn) {
    std::vector<std::vector<int>> mine = {
        {5},
        {10},
        {20}
    };
    EXPECT_EQ(goldMine(mine), 35); // All values are added from top to bottom
}

TEST(GoldMineTest, AllZeroes) {
    std::vector<std::vector<int>> mine = {
        {0, 0},
        {0, 0}
    };
    EXPECT_EQ(goldMine(mine), 0); // Nothing to collect
}

TEST(GoldMineTest, DiagonalMaxPath) {
    std::vector<std::vector<int>> mine = {
        {1, 2, 3},
        {0, 10, 0},
        {0, 0, 20}
    };
    EXPECT_EQ(goldMine(mine), 33); // 3 → 10 → 20
}


TEST(GoldMineTest, FromBook) {
    std::vector<std::vector<int>> mine = {
        {3, 2, 12,15,10},
        {6, 19, 7,11,17},
        {8, 5, 12,32,21},
        {3, 20, 2,9,7},
    };
    EXPECT_EQ(goldMine(mine), 73); // 15 → 17 → 32 ->9
}
