#include <vector>
#include "bit_manipulation/lonely_number.hpp"

// Implement your lonely_number logic here.
int lonelyNumber(std::vector<int> nums){
    int res=0;
    for (int num:nums){
        res^=num;
    }
    return res;
}
