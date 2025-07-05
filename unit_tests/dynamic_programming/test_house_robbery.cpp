#include "gtest/gtest.h"
#include "dynamic_programming/house_robbery.hpp"

TEST(HouseRobberyTest, EmptyArray) {
    std::vector<int> houses = {};
    EXPECT_EQ(houseRobbery(houses), 0);
}

TEST(HouseRobberyTest, SingleHouse) {
    std::vector<int> houses = {7};
    EXPECT_EQ(houseRobbery(houses), 7);
}

TEST(HouseRobberyTest, TwoHouses) {
    std::vector<int> houses = {2, 10};
    EXPECT_EQ(houseRobbery(houses), 10);
}

TEST(HouseRobberyTest, ThreeHouses) {
    std::vector<int> houses = {2, 7, 9};
    EXPECT_EQ(houseRobbery(houses), 11); // rob 2 + 9
}

TEST(HouseRobberyTest, LargerInput) {
    std::vector<int> houses = {2, 1, 1, 2};
    EXPECT_EQ(houseRobbery(houses), 4); // rob 2 + 2
}

TEST(HouseRobberyTest, ComplexCase) {
    std::vector<int> houses = {6, 7, 1, 30, 8, 2, 4};
    EXPECT_EQ(houseRobbery(houses), 41); // optimal: 7 + 30 + 4
}
