#include <vector>
#include "sorting/kth_largest_element.hpp"
#include <algorithm>
#include<iostream>
// Implement your kth_largest_element logic here.

int kthGratestValue(std::vector<int>nums,int k){
    std::sort(nums.begin(),nums.end(),std::greater<int>());
    return nums.at(k-1);
    
};
