
#include "gtest/gtest.h"
#include "recursion/has_adjacent_cells.hpp"

TEST(HasAdjacentCellsTest, BasicCases) {
    std::string s1 = "aabb";
    EXPECT_TRUE(hasAdjacentCells(s1, 1));  // 'a' == 'a'

    std::string s2 = "abcde";
    EXPECT_FALSE(hasAdjacentCells(s2, 1)); // no adjacent match

    std::string s3 = "aabbcc";
    EXPECT_TRUE(hasAdjacentCells(s3, 1));  // 'a' == 'a'

    std::string s4 = "abccde";
    EXPECT_TRUE(hasAdjacentCells(s4, 1));  // 'c' == 'c' at i = 3

    std::string s5 = "a";
    EXPECT_FALSE(hasAdjacentCells(s5, 1));  // too short

    std::string s6 = "";
    EXPECT_FALSE(hasAdjacentCells(s6, 1));  // empty string
}
