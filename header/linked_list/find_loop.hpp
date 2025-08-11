#pragma once

// Declare your find_loop interface here.
class LinkedListAE {
 public:
  int m_value;
  LinkedListAE* m_next;

  explicit LinkedListAE(int value);
};

//function prototype
LinkedListAE* findLoop(LinkedListAE* head);
