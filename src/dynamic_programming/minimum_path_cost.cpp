
#include <vector>
#include "iostream"
#include "dynamic_programming/minimum_path_cost.hpp"
#include "matrix/row_by_row.hpp"

// Implement your minimum_path_cost logic here.
int minimumPathCost(const std::vector<std::vector<int>>& grid){
    int rows=grid.size();
    int cols=grid[0].size();
    
    std::vector<std::vector<int>> dp (rows,std::vector<int>(cols,0));
    dp[0][0]=grid[0][0];
    //fill in first row
    for(int i=1;i<rows;++i){
        dp[i][0]=dp[i-1][0]+grid[i][0];
    }
    //fill in first row
    for (int j=1;j<cols;++j){
        dp[0][j]=dp[0][j-1]+grid[0][j];
    }
    std::cout<<"After removing first row and column"<<'\n';
    rowByRowPrint(dp);
    
    for (int r=1;r<rows;++r){
        for (int c=1;c<cols;++c){
            dp[r][c]=grid[r][c]+std::min(dp[r-1][c],dp[r][c-1]);
        }
    }
    return dp[rows-1][cols-1];
    
}
