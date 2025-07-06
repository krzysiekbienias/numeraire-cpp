#include<vector>
#include "dynamic_programming/knapsack_problem01.hpp"
#include "matrix/row_by_row.hpp"
#include <iostream>


// Implement your knapsack_problem01 logic here.
int knapscack01(const std::vector<int>& values, const std::vector<int>&weights, int k){
    if(k==0) return 0;
    
    int n=values.size();
    //dp container
    std::vector<std:: vector<int>> dp(n,std::vector<int>(k+1,0));
    rowByRowPrint(dp);
    std::cout<<"Base case"<<'\n';
    for (int i=weights[0];i<k+1;++i){
        dp[0][i]=values[0];
        
    }
    rowByRowPrint(dp);
    for (int i=1;i<n;++i){
        for (int j=0;j<k+1;++j){
            if(weights[i]>j){
                dp[i][j]=dp[i-1][j];
            }
            else{
                dp[i][j]=std::max(values[i]+dp[i-1][j-weights[i]],dp[i-1][j]);
            }
            
        }
    }
    return dp[n-1][k];
}
