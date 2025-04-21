#include<vector>
#include<iostream>
#include<map>

// Implement your minimum_loss logic here.

int minimumLoss(const std::vector<int>& prices) {
    std::map<int,size_t> originalIndices;
    for (size_t i=0;i<prices.size();i++) {
        originalIndices[prices[i]]=i;
    }
    //make copy of original index and sort it
    std::vector<int> sortedPrices=prices;
    std::sort(sortedPrices.begin(),sortedPrices.end());
    
    int minLoss=std::numeric_limits<int>::max();
    for (size_t i=1;i<sortedPrices.size();i++) {
        int lower=sortedPrices[i-1];
        int higher=sortedPrices[i];
        if (originalIndices[higher]<originalIndices[lower]) {
            std::cout <<"Valid combination buy for " <<higher<<" and sell for "<< lower <<std::endl;
            int current_loss=higher-lower; // loss will be positive
            std::cout << "The transaction generates loss equall "<<current_loss<<std::endl;
            if (current_loss<minLoss) {
                std::cout << "Current loss = "<<current_loss<<" is smaller than minimum loss observed so far so we replace it"<<std::endl;
                minLoss=current_loss;
            }
        }
        std::cout<<"Not valid transaction for "<< i <<" and"<<i-1<<"!"<<std::endl;
        
    }
    return minLoss;
}
