#include <vector>
#include <algorithm>

std::vector<int> twoNumberSum(std::vector<int>& array, int targetSum) {
    std::sort(array.begin() ,array.end());
    int low_p=0;
    int high_p;
    high_p = array.size() - 1;
    while (low_p<high_p) {
        int currentSum=array[low_p]+array[high_p];
        if (targetSum==currentSum){
            return {array[low_p],array[high_p]};
        }
        else if (currentSum<targetSum) {
            low_p++;
            
        }
        else {
            --high_p;
        }
    }
    return {};
}
