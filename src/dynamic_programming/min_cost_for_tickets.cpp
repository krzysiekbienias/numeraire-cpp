
#include <vector>
#include "dynamic_programming/min_cost_for_tickets.hpp"
#include <algorithm>


// Implement your min_cost_for_tickets logic here.
int minimumCostTickets(const std::vector<int>& trainDays, const std::vector<int> & costs, int n){
    //prepare container for track day when we need to commute
    std::vector <bool> isTrainDay(n+1,false);
    for(int day:trainDays){
        isTrainDay[day]=true;
    }
    std::vector<int>dp(n+1,0);
    for (int i=1;i<=n;++i){
        if (!isTrainDay[i]) {
            dp[i]=dp[i-1];//no need to buy a ticket
        }
        else{
            int dayCost=dp[std::max(0,i-1)]+costs[0]; //smart way to handle with i>=0
            int weekCost=dp[std::max(0,i-7)]+costs[1]; //smart way to handle with i>=7
            int monthCost=dp[std::max(0,i-30)]+costs[2]; //smart way to handle with i>=30
            dp[i]=std::min({dayCost,weekCost,monthCost});
        }
    }
    return dp[n-1];
}

