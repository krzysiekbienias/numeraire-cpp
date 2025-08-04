
#include "heap/continious_median.hpp"
#include <gtest/gtest.h>

TEST(ContinousMedianHandlerTest, InsertAndMedianOddCount) {
    ContinousMedianHandler cmh;
    cmh.insert(1);
    cmh.insert(2);
    cmh.insert(3);

    EXPECT_DOUBLE_EQ(cmh.getMedian(), 2.0);
    EXPECT_EQ(cmh.getMaxHeapTop(), 2);
    EXPECT_EQ(cmh.getMinHeapTop(), 3);
}

TEST(ContinousMedianHandlerTest, InsertAndMedianEvenCount) {
    ContinousMedianHandler cmh;
    cmh.insert(1);
    cmh.insert(2);
    cmh.insert(3);
    cmh.insert(4);

    EXPECT_DOUBLE_EQ(cmh.getMedian(), 2.5);
    EXPECT_EQ(cmh.getMaxHeapTop(), 2);
    EXPECT_EQ(cmh.getMinHeapTop(), 3);
}

TEST(ContinousMedianHandlerTest, InsertDuplicates) {
    ContinousMedianHandler cmh;
    std::vector<int> stream = {0, 1, 2, 2, 4, 5, 7, 8, 9};
    for (int n : stream) cmh.insert(n);

    EXPECT_DOUBLE_EQ(cmh.getMedian(), 4.0);
    EXPECT_EQ(cmh.getMaxHeapTop(), 4);
    EXPECT_EQ(cmh.getMinHeapTop(), 5);
}

TEST(ContinousMedianHandlerTest, ThrowsOnEmptyHeapAccess) {
    /// Regression test: These used to crash on empty stream/
    /// if anyone removes those if (heap.empty()) throw ... checks, this test will fail.
    ContinousMedianHandler cmh;
    EXPECT_THROW(cmh.getMedian(), std::runtime_error);
    EXPECT_THROW(cmh.getMaxHeapTop(), std::runtime_error);
    EXPECT_THROW(cmh.getMinHeapTop(), std::runtime_error);
}
