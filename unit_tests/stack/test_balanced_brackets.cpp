#include <gtest/gtest.h>
#include "stack/balanced_brackets.hpp"




TEST(BalancedBracketTest, EmptyString) {
    std::string s = "";
    EXPECT_TRUE(balancedBracket(s));
}

TEST(BalancedBracketTest, NoBrackets) {
    std::string s = "hello world!";
    EXPECT_TRUE(balancedBracket(s));
}

TEST(BalancedBracketTest, SingleBracket) {
    std::string s = "(";
    EXPECT_FALSE(balancedBracket(s));
}

TEST(BalancedBracketTest, SimpleBalanced) {
    std::string s = "()";
    EXPECT_TRUE(balancedBracket(s));
}

TEST(BalancedBracketTest, NestedBalanced) {
    std::string s = "{[()]}";
    EXPECT_TRUE(balancedBracket(s));
}

TEST(BalancedBracketTest, ComplexBalanced) {
    std::string s = "{[(hello)](world)}";
    EXPECT_TRUE(balancedBracket(s));
}

TEST(BalancedBracketTest, IncorrectOrder) {
    std::string s = "{[)]}";
    EXPECT_FALSE(balancedBracket(s));
}

TEST(BalancedBracketTest, MismatchedTypes) {
    std::string s = "{[}";
    EXPECT_FALSE(balancedBracket(s));
}

TEST(BalancedBracketTest, ExtraClosing) {
    std::string s = "()]";
    EXPECT_FALSE(balancedBracket(s));
}

TEST(BalancedBracketTest, ExtraOpening) {
    std::string s = "(()";
    EXPECT_FALSE(balancedBracket(s));
}
