#include "arrays/majority_element.hpp"
#include <vector>
#include <unordered_map>

// Implement your majority_element logic here.
int majorityElement(std::vector<int>& nums){
    int n=nums.size();
    std::unordered_map<int , int> elementOccurence;
    for(int num:nums){
        elementOccurence[num]++;
    }
    for (auto &[key,value]:elementOccurence){
        if (value>n/2) {
            return key;
        }
    }
    return -1;
}
