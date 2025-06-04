#include <vector>
#include "greedy/jump_to_the_end.hpp"


// Implement your jump_to_the_end logic here.
bool jumpToTheEnd(const std::vector<int> & nums){
    //we are goona traferse from the end
    int destination=nums.size()-1;
    for (int i=nums.size()-1;i>=0;--i){
        if (i+nums[i]>=destination){
            destination=i;
        }
        
    }
    return destination==0;
}
            
