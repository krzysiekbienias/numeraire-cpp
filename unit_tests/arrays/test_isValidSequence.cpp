#include <gtest/gtest.h>
#include "arrays/isValidSequence.hpp"



TEST(IsValidSequenceTest, FullMatch) {
    std::vector<int> array = {1, 2, 3, 4};
    std::vector<int> sequence = {1, 2, 3, 4};
    EXPECT_TRUE(isValidSequence(array, sequence));
}

TEST(IsValidSequenceTest, SubsequenceMatch) {
    std::vector<int> array = {5, 1, 2, 3, 9};
    std::vector<int> sequence = {1, 2, 3};
    EXPECT_TRUE(isValidSequence(array, sequence));
}

TEST(IsValidSequenceTest, OutOfOrderSequence) {
    std::vector<int> array = {1, 2, 3, 4};
    std::vector<int> sequence = {2, 1};
    EXPECT_FALSE(isValidSequence(array, sequence));
}

TEST(IsValidSequenceTest, ExtraElementInSequence) {
    std::vector<int> array = {1, 2, 3, 4};
    std::vector<int> sequence = {2, 3, 4, 5};
    EXPECT_FALSE(isValidSequence(array, sequence));
}

TEST(IsValidSequenceTest, EmptySequence) {
    std::vector<int> array = {1, 2, 3};
    std::vector<int> sequence = {};
    EXPECT_TRUE(isValidSequence(array, sequence));
}

TEST(IsValidSequenceTest, EmptyArray) {
    std::vector<int> array = {};
    std::vector<int> sequence = {1};
    EXPECT_FALSE(isValidSequence(array, sequence));
}

TEST(IsValidSequenceTest, IdenticalButShorter) {
    std::vector<int> array = {1, 2, 3, 4};
    std::vector<int> sequence = {1, 2, 4};
    EXPECT_TRUE(isValidSequence(array, sequence));
}

TEST(IsValidSequenceTest, IdenticalValuesNonSequential) {
    std::vector<int> array = {1, 2, 1, 2, 3};
    std::vector<int> sequence = {1, 2, 3};
    EXPECT_TRUE(isValidSequence(array, sequence));
}
