
#include "gtest/gtest.h"
#include "famous_algorithms/kadanes_algorithm.hpp"

TEST(KadanesAlgorithmTest, HandlesEmptyArray) {
    std::vector<int> arr = {};
    EXPECT_EQ(kadanesAlgorithm(arr), 0);
}

TEST(KadanesAlgorithmTest, HandlesSingleElement) {
    EXPECT_EQ(kadanesAlgorithm({5}), 5);
    EXPECT_EQ(kadanesAlgorithm({-3}), -3);
}

TEST(KadanesAlgorithmTest, HandlesAllPositive) {
    std::vector<int> arr = {1, 2, 3, 4};
    EXPECT_EQ(kadanesAlgorithm(arr), 10);
}

TEST(KadanesAlgorithmTest, HandlesAllNegative) {
    std::vector<int> arr = {-1, -2, -3, -4};
    EXPECT_EQ(kadanesAlgorithm(arr), -1);
}

TEST(KadanesAlgorithmTest, HandlesMixedValues) {
    std::vector<int> arr = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    EXPECT_EQ(kadanesAlgorithm(arr), 6);  // subarray: [4, -1, 2, 1]
}

TEST(KadanesAlgorithmTest, HandlesLargerPeaksLater) {
    std::vector<int> arr = {-5, -1, 2, 3, 4, -10, 100};
    EXPECT_EQ(kadanesAlgorithm(arr), 100);  // peak at the end
}
