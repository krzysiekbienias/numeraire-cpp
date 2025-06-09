#include <vector>

// Implement your kadanes_algorithm logic here.
int kadanesAlgorithm(const std::vector<int>& array){
    if(array.empty()) return 0;
    int maxSoFar=array[0];
    int currentMax=array[0];
    for (int i=1;i<array.size();++i){
        currentMax=std::max(array[i],currentMax+array[i]);
        maxSoFar=std::max(currentMax,maxSoFar);
    }
    return maxSoFar;
}

