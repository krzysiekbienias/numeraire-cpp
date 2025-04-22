
#include <gtest/gtest.h>
#include "recursion/nth_fibbonaci.hpp"

// Base case: n = 0
TEST(NthFibonacciTest, Zero) {
    std::unordered_map<int, int> memo;
    EXPECT_EQ(nthFibbonaci(0, memo), 0);
}

// Base case: n = 1
TEST(NthFibonacciTest, One) {
    std::unordered_map<int, int> memo;
    EXPECT_EQ(nthFibbonaci(1, memo), 1);
}

// Small n = 2
TEST(NthFibonacciTest, Two) {
    std::unordered_map<int, int> memo;
    EXPECT_EQ(nthFibbonaci(2, memo), 1);
}

// Small n = 5
TEST(NthFibonacciTest, Five) {
    std::unordered_map<int, int> memo;
    EXPECT_EQ(nthFibbonaci(5, memo), 5);
}

// Medium n = 10
TEST(NthFibonacciTest, Ten) {
    std::unordered_map<int, int> memo;
    EXPECT_EQ(nthFibbonaci(10, memo), 55);
}

// Larger n = 20
TEST(NthFibonacciTest, Twenty) {
    std::unordered_map<int, int> memo;
    EXPECT_EQ(nthFibbonaci(20, memo), 6765);
}

// Repeated call should hit memo
TEST(NthFibonacciTest, MemoReuse) {
    std::unordered_map<int, int> memo;
    EXPECT_EQ(nthFibbonaci(15, memo), 610);
    EXPECT_EQ(memo.count(15), 1);
    EXPECT_EQ(memo[15], 610);
    EXPECT_GT(memo.size(), 1); // Intermediate values cached
}
