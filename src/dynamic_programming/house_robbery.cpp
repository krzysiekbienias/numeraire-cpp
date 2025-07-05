#include <vector>
#include <iostream>
// Implement your house_robbery logic here.

int houseRobbery(const std::vector<int> &array){
    size_t n =array.size();
    if (n==0) return 0;
    if (n==1) return array[0];
    if (n==2) return std::max(array[0],array[1]);
    
    //If you’re building a dp table, even for small input, you must assign the initial base values before using them in recurrence.
    std::vector<int>dp(n,0);
    dp[0]=array[0];
    dp[1]=std::max(array[0],array[1]);
    
    //Now we may loop and fill the rest
    for (size_t i=2;i<n;++i){
        dp[i]=std::max(dp[i-2]+array[i],dp[i-1]);
    }
    return dp[n-1];
    
}
