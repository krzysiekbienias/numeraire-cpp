#pragma once

// Declare your single_linked_list interface here.
#include <iostream>

class LinkedList; // forward declare if needed
namespace print_utils {
    
    void printLinkedList(const LinkedList& list, const std::string& label);
}



struct Node{
    int value;
    Node* next;
    
    Node(int val): value(val),next (nullptr){};
};




class LinkedList{
    
private:
    Node * head;
public:
    LinkedList(); // constructor
    ~LinkedList(); //
    int size() const;
    bool isEmpty() const;
    void insertAtEnd(int value);
    void insertAtFront(int value);
    int getMiddle() const;
    bool search(int value) const;
    void removeTail();
    void removeKthNode(int k);
    
    
    friend void print_utils::printLinkedList(const LinkedList& list, const std::string& label);
};

