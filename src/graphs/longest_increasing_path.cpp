#include <vector>
#include <iostream>
#include "matrix/is_within_bounds.hpp"
#include "graphs/longest_increasing_path.hpp"

////becareful becasue usually dfs is void here is int
//int dfs(int r, int c, const std::vector<std::vector<int>>& matrix,std::vector<std::vector<int>>& dp){
//    int row=matrix.size();
//    int cols=matrix[0].size();
//    int maxPath=0;
//    
//    std::vector<std::pair<int, int>> directions={{-1,0},{1,0},{0,-1},{0,1}};
//    for (auto & [dr,dc]:directions){
//        int newRow=r+dr;
//        int newCol=c+dc;
//        if (isWithinBounds(newRow, newCol, matrix) && matrix[newRow][newCol]>matrix[r][c]) {
//            maxPath=std::max(maxPath,1+dfs(newRow, newCol, matrix,dp));
//        }
//    }
//    
//    
//    
//}
//
//// Implement your longest_increasing_path logic here.
//int longestIncreasingPath(const std::vector<std::vector<int>>& matrix){
//    std::vector<std::vector<int>> dp(rows,std::vector<int>(cols,0));
//    
//    
//    int pathLength=0
//    
//    
//    
//}
