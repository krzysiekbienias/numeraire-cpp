
#include "dynamic_programming/paths_in_matrix.hpp"
#include <vector>
#include <iostream>
#include "matrix/row_by_row.hpp"

// Implement your paths_in_matrix logic here.

int pathsInMatrix(const std::vector<std::vector<int>>& grid){
    int rows =grid.size();
    int cols=grid[0].size();
    std::vector<std::vector<int>> dp(rows,std::vector<int>(cols,0));
    if (grid[0][0]==1){
        return 0;
    }
    dp[0][0]=1;
    //fill in first row
    for (int i=1;i<cols;++i){
        if(grid[0][i]==0){
            dp[0][i]=dp[0][i-1];
        }
        else
            dp[0][i]=0; //we are block by the wall
    }
    //fill in first column
    for (int j=1;j<rows;++j){
        if (grid[j][0]==0){
            dp[j][0]=dp[j-1][0];
            
        }
        else{
            dp[j][0]=0;
        }
    }
    std::cout<<"After Filling in first row and column."<<'\n';
    
    rowByRowPrint(dp);
    for (int r=1;r<rows;r++){
        for (int c=1;c<cols;c++){
            if (grid[r][c]==0){
                dp[r][c]=dp[r-1][c]+dp[r][c-1];
            }
            else{
                dp[r][c]=0;
            }
        }
    }
    std::cout<<"After Complete Traversal"<<'\n';
    rowByRowPrint(dp);
    return dp[rows-1][cols-1];
    
}


