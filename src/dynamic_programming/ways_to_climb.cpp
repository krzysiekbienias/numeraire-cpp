#include <vector>
#include <iostream>


// Implement your ways_to_climb logic here.
int waysToClimb(int n,const std::vector<int>& jumps){
    std::vector<int> dp(n+1,0);
    dp[0]=1; // There’s 1 way to reach step 0: do nothing.
    for (size_t i=1;i<n+1;++i){
        for (int jump:jumps){
            if(i-jump<0)continue; //save guard becasue without it failed one test: EXPECT_EQ(waysToClimb(1, {1, 2}), 1)
            if(i>=jump){
                dp[i]+=dp[i-jump];
            }
        }
    }
    return dp[n];
}
