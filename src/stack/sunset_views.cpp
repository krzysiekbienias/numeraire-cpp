#include <vector>
#include <string>
#include "stack/sunset_views.hpp"
#include <stack>
// Implement your sunset_views logic here.

std::vector<int> sunsetViews(std::vector<int> buildings, std::string direction) {
    std::vector <int> result;
    int maxHeightSoFar=INT_MIN;
    if(direction=="WEST"){
        for (int i=0;i<buildings.size();++i){
            if (maxHeightSoFar<buildings[i]){
                result.push_back(i);
                maxHeightSoFar=buildings[i];
            }
        }
    }
    else if(direction=="EAST"){
        for (int i=buildings.size()-1;i>=0;--i){
            if(maxHeightSoFar<buildings[i]){
                result.push_back(i);
                maxHeightSoFar=buildings[i];
            }
        }
        std::reverse(result.begin(),result.end());
        
    }
    return result;
}


        
