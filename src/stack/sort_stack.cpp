#include "stack/sort_stack.hpp"
#include <vector>

// Implement your sort_stack logic here.
std::vector<int> sortStack(std::vector<int> &stack) {
  if (stack.empty())
    return stack;
  int top = stack.back();
  stack.pop_back();
  sortStack(stack);
  insertInSortedOrder(stack, top);
    return stack;
}

void insertInSortedOrder(std::vector<int> &stack, int value) {
  if (stack.size() == 0 || stack.back() <= value) {
    stack.push_back(value);
    return;
  }
  int top = stack.back();
  stack.pop_back();
  insertInSortedOrder(stack, value);
  stack.push_back(top); // this line is essential to restore full structure.
}
