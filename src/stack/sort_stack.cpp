#include<vector>
#include "stack/sort_stack.hpp"

// Implement your sort_stack logic here.
void sortStack(std::vector<int>& stack){
    if (stack.size()==0) return;
    int top = stack.back();
    stack.pop_back();
    sortStack(stack);
    insertInSortedOrder(stack,top);
    
}


void insertInSortedOrder(std::vector<int>& stack ,int value){
    if (stack.size()==0 || stack.back()<=value){
        stack.push_back(value);
        return ;
    }
    int top =stack.back();
    stack.pop_back();
    insertInSortedOrder(stack,value);
    stack.push_back(top); //this line is essential to restore full structure.
}

