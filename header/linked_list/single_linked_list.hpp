#pragma once

// Declare your single_linked_list interface here.
#include <iostream>

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
    
    
    
};


