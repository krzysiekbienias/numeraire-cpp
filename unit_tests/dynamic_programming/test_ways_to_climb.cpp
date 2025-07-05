#include "gtest/gtest.h"
#include "dynamic_programming/ways_to_climb.hpp"

TEST(WaysToClimbTest, BaseCases) {
    EXPECT_EQ(waysToClimb(0, {1, 2}), 1);  // 1 way to do nothing
    EXPECT_EQ(waysToClimb(1, {1, 2}), 1);  // [1]
    EXPECT_EQ(waysToClimb(2, {1, 2}), 2);  // [1+1], [2]
}

TEST(WaysToClimbTest, ThreeSteps) {
    EXPECT_EQ(waysToClimb(3, {1, 2}), 3);  // [1+1+1], [1+2], [2+1]
}

TEST(WaysToClimbTest, StepsWithJumpSet) {
    EXPECT_EQ(waysToClimb(4, {1, 3, 5}), 3);  // [1+1+1+1], [1+3], [3+1]
    EXPECT_EQ(waysToClimb(5, {2, 3}), 2);     // [2+3], [3+2]
}

TEST(WaysToClimbTest, NoValidJumps) {
    EXPECT_EQ(waysToClimb(4, {5, 6}), 0);     // no way to climb
}

TEST(WaysToClimbTest, LargeStepWithOneJump) {
    EXPECT_EQ(waysToClimb(10, {2}), 1);       // only one path: 2+2+2+2+2
}
