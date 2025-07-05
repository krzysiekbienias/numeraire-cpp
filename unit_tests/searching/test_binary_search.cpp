
#include <gtest/gtest.h>
#include "searching/binary_search.hpp"


// Test: Target is in the middle
TEST(BinarySearchTest, FoundInMiddle) {
    std::vector<int> vec = {1, 3, 5, 7, 9};
    EXPECT_EQ(binary_search(vec, 5), 2);
}

// Test: Target is the first element
TEST(BinarySearchTest, FoundAtStart) {
    std::vector<int> vec = {1, 3, 5, 7, 9};
    EXPECT_EQ(binary_search(vec, 1), 0);
}

// Test: Target is the last element
TEST(BinarySearchTest, FoundAtEnd) {
    std::vector<int> vec = {1, 3, 5, 7, 9};
    EXPECT_EQ(binary_search(vec, 9), 4);
}

// Test: Target is not in the array
TEST(BinarySearchTest, NotFound) {
    std::vector<int> vec = {1, 3, 5, 7, 9};
    EXPECT_EQ(binary_search(vec, 8), -1);
}

// Test: Empty array
TEST(BinarySearchTest, EmptyArray) {
    std::vector<int> vec = {};
    EXPECT_EQ(binary_search(vec, 5), -1);
}

// Test: Single element, match
TEST(BinarySearchTest, SingleElementMatch) {
    std::vector<int> vec = {5};
    EXPECT_EQ(binary_search(vec, 5), 0);
}

// Test: Single element, no match
TEST(BinarySearchTest, SingleElementNoMatch) {
    std::vector<int> vec = {5};
    EXPECT_EQ(binary_search(vec, 3), -1);
}

// Test: Duplicates
TEST(BinarySearchTest, Duplicates) {
    std::vector<int> vec = {1, 2, 4, 4, 4, 5, 6};
    int result = binary_search(vec, 4);
    EXPECT_GE(result, 2);
    EXPECT_LE(result, 4);
}
