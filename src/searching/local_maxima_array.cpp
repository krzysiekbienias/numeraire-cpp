#include <vector>
#include "searching/local_maxima_array.hpp"

// Implement your local_maxima_array logic here.
int localMaximaInArrayLinear(const std::vector<int>& nums){
    int n=nums.size();
    if (nums.size()==1) return 0; //<--- only one element
    // Check if first element is a local maxim
    // IMPORTANT We leverage from the fact firs local max is accepted.
    if (nums[0]>nums[1]) return 0;
    for(int i=1;i<n-1;++i){
        if(nums[i]>nums[i+1] && nums[i]>nums[i-1]){
            return i;
        }
    }
    if (nums[n-2]<nums[n-1]) return n-1;
    //without this we need some fallback return for instance return -1;
    throw std::runtime_error("No local maximum found");
}
