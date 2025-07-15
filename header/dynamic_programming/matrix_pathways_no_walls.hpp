#pragma once
#include<vector>

// Declare your matrix_pathways_no_walls interface here.
int matrixPathways(int m,int n){
    std::vector<std::vector<int>> dp(m,std::vector<int>(n,1));
    //how base case
    //we fill in grid with ones. In particular we need first row and first col =1 becaue once we leave row or col we can never return we can only move right or down, so therr is only one path to those cells.
    //|111
     //1
     //1
    
    for (int i=1;i<m;++i){
        for (int j=1;j<n;j++){
            dp[i][j]=dp[i-1][j]+dp[i][j-1];
        }
    }
    return dp[m-1][n-1];
}
