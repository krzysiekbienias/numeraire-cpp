#include "linked_list/single_linked_list.hpp"

// Constructor
LinkedList::LinkedList() : head(nullptr) {}

// Desctructor

LinkedList::~LinkedList() {
  Node *current = head;
  while (current != nullptr) {
    Node *nextNode = current->next;
    delete current;
    current = nextNode;
  }
}

int LinkedList::size() const {
  int count = 0;
  Node *current = head;
  while (current != nullptr) {
    ++count;
    current = current->next;
  }
  return count;
}

bool LinkedList::isEmpty() const { return head == nullptr; }

void LinkedList::insertAtEnd(int value) {
  Node *newNode = new Node(value);
  if (head == nullptr) {
    head = newNode;
    return;
  }
  // Traverse till end of ll
  Node *current = head;
  while (current->next != nullptr) {
    current = current->next;
  }
  current->next = newNode;
}

void LinkedList::insertAtFront(int value) {
  Node *newNode = new Node(value);
  if (head == nullptr) {
    head = newNode;
    return;
  }
  newNode->next = head;
  head = newNode;
}

int LinkedList::getMiddle() const {
  if (!head) {
    throw std::runtime_error("List is empty");
  }

  Node *slow = head;
  Node *fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }
  return slow->value;
}

bool LinkedList::search(int value) const {
  Node *current = head;
  while (current != nullptr) {
    if (current->value == value) {
      return true;
    }
    current = current->next;
  }
  return false;
}

void LinkedList::removeTail() {
  if (head == nullptr or head->next == nullptr) {
    head = nullptr;
    return;
  }
  Node *current = head;
  while (current->next != nullptr && current->next->next != nullptr) {
    current = current->next;
  }
  current->next = nullptr;
}

void LinkedList::removeKthNode(int k) {
  // I need dummy node becasue it may happen that head itself is the node we
  // need to remove

  Node *dummy = new Node(-1);
  dummy->next = head;
  Node *leader = dummy;
  Node *trailer = dummy;
  for (int i = 0; i < k; ++i) {
    if (leader->next == nullptr) {
      delete dummy; // we need to revert to orginal version
      return;       // k is too large nothing to remove
    }
    leader = leader->next;
  }
  while (leader->next != nullptr) {
    leader = leader->next;
    trailer = trailer->next;
  }
  // if we get to the end with leader it means that trailer is one node before k
  Node *toDelete = trailer->next;
  trailer->next = toDelete->next;
  delete toDelete;
  // we also need to get back to default settings where head is a first node,
  // not a dummy node.
  head = dummy->next;
  delete dummy;
}

void LinkedList::removeDuplicates() {
  Node *current = head;
  while (current!=nullptr && current->next != nullptr) {
      if (current->value == current->next->value) {
          Node * duplicate=current->next;
          current->next=current->next->next;
          delete duplicate;
      }
      else{
          current=current->next;
      }
    }
  }



