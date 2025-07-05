#include <vector>
#include "dynamic_programming/gold_mine.hpp"
#include "matrix/is_within_bounds.hpp"

int goldMine(const std::vector<std::vector<int>>& mine){
    size_t rows=mine.size();
    size_t columns=mine[0].size();
    std::vector<std::vector<int>>dp(rows,std::vector<int>(columns,0));
    //first row
    for(size_t c=0;c<columns;++c){
        dp[0][c]=mine[0][c];
    }
    for (size_t r=1;r<rows;++r){
        for (size_t c=0;c<columns;++c){
            int top_left=(c>0) ? dp[r-1][c-1]:0;
            int top=dp[r-1][c];
            int top_right=c+1<columns ? dp[r-1][c+1]:0;
            dp[r][c]=mine[r][c]+std::max({top_left,top,top_right});
            
        }
    }
    return *std::max_element(dp[rows-1].begin(), dp[rows-1].end());
}
