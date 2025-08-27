#include <gtest/gtest.h>
#include "strings/roman_to_intiger.hpp"


TEST(RomanToIntegerTest, BasicConversions) {
    EXPECT_EQ(romanToIntiger("III"), 3);
    EXPECT_EQ(romanToIntiger("IV"), 4);
    EXPECT_EQ(romanToIntiger("IX"), 9);
    EXPECT_EQ(romanToIntiger("LVIII"), 58);     // L = 50, V = 5, III = 3
    EXPECT_EQ(romanToIntiger("MCMXCIV"), 1994); // M=1000, CM=900, XC=90, IV=4
}

TEST(RomanToIntegerTest, EdgeCases) {
    EXPECT_EQ(romanToIntiger("I"), 1);
    EXPECT_EQ(romanToIntiger("MMMCMXCIX"), 3999); // Largest valid Roman numeral
    EXPECT_EQ(romanToIntiger(""), 0);
}
