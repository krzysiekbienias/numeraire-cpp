#include "greedy/tandem_bicycle.hpp"
#include <vector>


// Implement your tandem_bicycle logic here.
int tandemBicycle(std::vector<int> redShirtSpeeds, std::vector<int> blueShirtSpeeds, bool fastest){
    int results=0;
    std::sort(redShirtSpeeds.begin(), redShirtSpeeds.end());
    if(fastest){
    std::sort(blueShirtSpeeds.begin(), blueShirtSpeeds.end(),std::greater<int>());
    for (int i=0;i<redShirtSpeeds.size();++i){
            results+=std::max(redShirtSpeeds[i],blueShirtSpeeds[i]);
        }
    }
    else{
        std::sort(blueShirtSpeeds.begin(), blueShirtSpeeds.end());
    for (int i=0;i<redShirtSpeeds.size();++i){
                results+=std::max(redShirtSpeeds[i],blueShirtSpeeds[i]);
            }
    }
    return results;
}

