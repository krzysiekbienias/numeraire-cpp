#include<vector>
#include"arrays/min_reward.hpp"
#include<numeric>
#include "utils/print_utils.hpp"

// Implement your min_reward logic here.
    int minReward(const std::vector<int>& scores){
        std::vector<int> rewards = std::vector<int> (scores.size(),1);
        //if  current score is bigger than previous one just increase current rewards, with respect to previous
        for(int i=1;i<scores.size();++i){
            if (scores[i]>scores[i-1]) {
                rewards[i]=rewards[i-1]+1;
            }
        }
        //if current rewards is bigger than next one then make sure that current is bigger
        for(int j=scores.size()-1;j=0;--j){
            if (scores[j]>scores[j+1]){
                rewards[j]=rewards[j+1]+1;
            }
        }
//        print_utils::printVector(rewards);
//        return std::accumulate(rewards.begin(),rewards.end(),0);
}
