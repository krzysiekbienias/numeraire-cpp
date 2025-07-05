#include<vector>
#include "dynamic_programming/lis_len.hpp"


// Implement your lis_len logic here.
int longestIncreasingSubsequence(const std::vector<int> &arr ){
    int n=arr.size();
    // edge case
    
    if (n==0) return 0;
    // dp[i] stores the length of the LIS ending at index i
    std::vector<int> dp(arr.size(),1);
    for (int i=1;i<n;++i){
        for (int j=0;j<i;++j){
            if(arr[j]<arr[i]){
                dp[i]=std::max(dp[i],dp[j]+1);
            }
        }
    }
    return *std::max_element(dp.begin(),dp.end());
}
