#include "sorting/dutch_national_flag.hpp"
#include <vector>

// Implement your dutch_national_flag logic here.
void dutchNationalFlag(std::vector<int>& nums){
    int i=0;
    int left=0;
    int right=nums.size()-1;
    while (i<=right){
        if (nums[i]==0) {
            std::swap(nums[i], nums[left]);
            left++;
            i++;
        }
        else if (nums[i]==2){
            std::swap(nums[i], nums[right]);
            right--;
        }
        else{
            i++;
        }
    }
}
