#include "arrays/longest_increasing_matrix_path.hpp"
#include <vector>
#include <algorithm>
#include <functional>



// Implement your longest_increasing_matrix_path logic here.
int longestIncreasingMatrixPath(const std::vector<std::vector<int>>& matrix){
    int rows=matrix.size();
    int cols=matrix[0].size();
    
    std::vector<std::pair<int, int>> directions={{-1,0},{1,0},{0,-1},{0,1}};
    std::vector<std::vector<int>> dp(rows,std::vector<int>(cols,0));
    
    std::function<int(int,int)> dfs=[&](int i, int j)->int{
        if (dp[i][j]!=0) return dp[i][j]; //already computed
        int maxLen=1;
        for (auto & dir:directions){
            int dx=dir.first;
            int dy=dir.second;
            int ni=i+dx;
            int nj=j+dy;
            //standard limits checking+ incresing
            if (ni>=0 && ni<rows && nj>=0 && nj<cols && matrix[ni][nj]>matrix[i][j]){
                maxLen=std::max(maxLen,1+dfs(ni,nj));
            }
        }
        dp[i][j]=maxLen;
        return maxLen;
    };
    
    int result=0;
    for (int i=0;i<rows;++i){
        for(int j=0;j<cols;++j){
            result=std::max(result,dfs(i,j));
        }
    }
    return result;
}


