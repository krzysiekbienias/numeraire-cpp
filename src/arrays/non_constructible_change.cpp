#include "arrays/non_constructible_change.hpp"
#include <vector>

// Implement your non_constructible_change logic here.
int nonConstructibleChange(std::vector<int> coins){
    if (coins.empty()){
        return 1;
    }
    
    std::sort(coins.begin(), coins.end());
    int currentChange=0;
    for (auto coin:coins){
        if(coin>currentChange+1){
            return currentChange+1;
        }
        else{
            currentChange+=coin;
        }
    }
    return currentChange+1;
}
