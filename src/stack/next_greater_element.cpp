#include "stack/next_greater_element.hpp"
#include <vector>

// Implement your next_greater_element logic here.
std::vector<int> nextGreaterElement(const std::vector<int> &array ){
    std::vector<int> stack;
    std::vector <int> results(array.size(),0);
    for (auto it =array.crbegin();it!=array.crend();++it){
        int idx=std::distance(it, array.crend())-1;
        while (!stack.empty() && array[idx]>=stack.back()) { //remove all candidates less or equal array[idx]
            stack.pop_back();
        }
        results[idx]=(!stack.empty()) ?stack.back():-1;
        stack.push_back(array[idx]);
    }
    
    return results;
}
