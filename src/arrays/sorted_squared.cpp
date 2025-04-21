#include<vector>


// Implement your sorted_squared logic here.
std::vector<int> sortedSquared(const std::vector<int>& array){
    std::vector<int> sortedArray;
    sortedArray.reserve(array.size());
    for (int num : array){
        sortedArray.push_back(num*num);
    }
    std::sort(sortedArray.begin(),sortedArray.end());
    return sortedArray;
    
    
}
