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

//You don’t need to manually delete anything because the LinkedList destructor is automatically called when each LinkedList object goes out of scope at the end of each test.
