#include <gtest/gtest.h>
#include "linked_list/single_linked_list.hpp" // Make sure you include your LinkedList header

// Test the constructor
TEST(LinkedListTest, ConstructorInitializesEmptyList) {
    LinkedList list;
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.size(), 0);
}

// Test isEmpty on a non-empty list
TEST(LinkedListTest, IsEmptyReturnsFalseAfterInsertion) {
    LinkedList list;
    list.insertAtEnd(10);  // we assume you have/will implement insertAtEnd
    EXPECT_FALSE(list.isEmpty());
}

// Test size after multiple insertions
TEST(LinkedListTest, SizeAfterInsertions) {
    LinkedList list;
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    EXPECT_EQ(list.size(), 3);
}

// Destructor test (indirect test for memory leaks)
TEST(LinkedListTest, DestructorCleansUpMemory) {
    LinkedList* list = new LinkedList();
    list->insertAtEnd(5);
    list->insertAtEnd(10);
    delete list; // Destructor should free all nodes
    
    // No direct test for destructor side-effects,
    // but if the program doesn't crash or leak memory (use Valgrind),
    // we assume success.
}


TEST(LinkedListTest, GetMiddleSingleElement) {
    LinkedList list;
    list.insertAtEnd(42);
    EXPECT_EQ(list.getMiddle(), 42);
}

TEST(LinkedListTest, GetMiddleOddElements) {
    LinkedList list;
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    list.insertAtEnd(4);
    list.insertAtEnd(5);
    EXPECT_EQ(list.getMiddle(), 3);
}

TEST(LinkedListTest, GetMiddleEvenElements) {
    LinkedList list;
    list.insertAtEnd(10);
    list.insertAtEnd(20);
    list.insertAtEnd(30);
    list.insertAtEnd(40);
    EXPECT_EQ(list.getMiddle(), 30); // Assuming we return the second middle node
}

TEST(LinkedListTest, GetMiddleEmptyList) {
    LinkedList list;
    EXPECT_THROW(list.getMiddle(), std::runtime_error);
}

TEST(LinkedListTest, Search_EmptyList_ReturnsFalse) {
    LinkedList list;
    EXPECT_FALSE(list.search(10));
}

TEST(LinkedListTest, Search_ValueExists_ReturnsTrue) {
    LinkedList list;
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    list.insertAtEnd(4);
    list.insertAtEnd(5);
    EXPECT_TRUE(list.search(3));
}

TEST(LinkedListTest, Search_ValueDoesNotExist_ReturnsFalse) {
    LinkedList list;
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    list.insertAtEnd(4);
    list.insertAtEnd(5);
    EXPECT_FALSE(list.search(6));
}

TEST(LinkedListTest, Search_FirstAndLastElement) {
    LinkedList list;
    list.insertAtEnd(10);
    list.insertAtEnd(20);
    list.insertAtEnd(30);
    EXPECT_TRUE(list.search(10));
    EXPECT_TRUE(list.search(30));
}

// --- Unit tests for remove_tail() ---

TEST(LinkedListTest, RemoveTail_EmptyList_NoCrash) {
    LinkedList list;
    list.remove_tail();
    EXPECT_FALSE(list.search(0)); // Should still be empty
}

TEST(LinkedListTest, RemoveTail_SingleNodeList) {
    LinkedList list;
    list.insertAtEnd(99);
    list.remove_tail();
    EXPECT_FALSE(list.search(99));
}

TEST(LinkedListTest, RemoveTail_MultipleNodes) {
    LinkedList list;
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    list.remove_tail();
    EXPECT_FALSE(list.search(3));
    EXPECT_TRUE(list.search(1));
    EXPECT_TRUE(list.search(2));
}

TEST(LinkedListTest, RemoveTail_Twice) {
    LinkedList list;
    list.insertAtEnd(1);
    list.insertAtEnd(2);
    list.insertAtEnd(3);
    list.insertAtEnd(4);
    list.remove_tail(); // Removes 4
    list.remove_tail(); // Removes 3
    EXPECT_FALSE(list.search(4));
    EXPECT_FALSE(list.search(3));
    EXPECT_TRUE(list.search(1));
    EXPECT_TRUE(list.search(2));
}


//You don’t need to manually delete anything because the LinkedList destructor is automatically called when each LinkedList object goes out of scope at the end of each test.
