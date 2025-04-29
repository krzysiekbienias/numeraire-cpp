#include <vector>

std::vector<int> smallestDifference(std::vector<int> &arrayOne, std::vector<int> &arrayTwo) {
    std ::sort(arrayOne.begin(),arrayOne.end());
    std ::sort(arrayTwo.begin(),arrayTwo.end());
    size_t i=0;
    size_t j=0;
    int minDiff=std::numeric_limits<int>::max();
    std::vector<int> closestPair;
    while (i<arrayOne.size() &&j<arrayTwo.size()) {
        int currentDiff=std::abs(arrayOne[i]-arrayTwo[j]);
        if (currentDiff==0) {
            return {arrayOne[i],arrayTwo[j]};
        }
        if(currentDiff<minDiff) {
            minDiff=currentDiff;
            closestPair={arrayOne[i],arrayTwo[j]};
        };
        if (arrayOne[i]<arrayTwo[j]) {
            i++;
        }
        else{
            j++;
            
        }
        
    }
    return closestPair;
}
