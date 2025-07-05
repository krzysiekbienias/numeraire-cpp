#include <vector>
#include "dynamic_programming/gold_mine.hpp"
#include "matrix/is_within_bounds.hpp"

// Implement your gold_mine logic here.
int goldMine(const std::vector<std::vector<int>> & mine){
    int rows =mine.size();
    int cols=mine[0].size();
    std::vector<std::vector<int>> dp(rows,std::vector<int>(cols,0));
    //we start with fill in the first row
    for (int i=0;i<cols;++i){
        dp[0][i]=mine[0][i];
    }
    
    for (int r=1;r<rows;++r){
        for (int c=0;c<cols;++c){
            dp[r][c]=mine[r][c]+std::max({dp[r-1][c],dp[r-1][c+1],dp[r-1][c-1]});
        }
    
    }
    int maxElementFromLastRow=*std::max_element(dp[rows-1].begin(), dp[rows-1].end());
    return maxElementFromLastRow;
}
