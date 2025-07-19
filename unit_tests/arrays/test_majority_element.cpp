#include <gtest/gtest.h>
#include <arrays/majority_element.hpp>

TEST(MajorityElementTest, MajorityExistsOddLength) {
    std::vector<int> nums = {3, 3, 4, 2, 3};
    EXPECT_EQ(majorityElement(nums), 3);
}

TEST(MajorityElementTest, MajorityExistsEvenLength) {
    std::vector<int> nums = {2, 2, 1, 1, 2, 2};
    EXPECT_EQ(majorityElement(nums), 2);
}

TEST(MajorityElementTest, NoMajorityExists) {
    std::vector<int> nums = {1, 2, 3, 4, 5};
    EXPECT_EQ(majorityElement(nums), -1);
}

TEST(MajorityElementTest, SingleElement) {
    std::vector<int> nums = {10};
    EXPECT_EQ(majorityElement(nums), 10);
}

TEST(MajorityElementTest, EmptyVector) {
    std::vector<int> nums = {};
    EXPECT_EQ(majorityElement(nums), -1);
}

TEST(MajorityElementTest, MajorityElementAtEnd) {
    std::vector<int> nums = {1, 2, 3, 4, 4, 4, 4};
    EXPECT_EQ(majorityElement(nums), 4);
}

TEST(MajorityElementTest, AllSameElements) {
    std::vector<int> nums = {7, 7, 7, 7, 7};
    EXPECT_EQ(majorityElement(nums), 7);
}
