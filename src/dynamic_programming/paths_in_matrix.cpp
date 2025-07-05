#include "dynamic_programming/paths_in_matrix.hpp"
#include <vector>
#include "matrix/row_by_row.hpp"
#include "utils/print_utils.hpp"

// Implement your paths_in_matrix logic here.
int pathsInMatrix(const std::vector<std::vector<int>> matrix){
    int rows =matrix.size();
    int cols = matrix[0].size();
    std::vector<std::vector<int>> dp(rows,std::vector<int>(cols,0));
    bool wallSeen=false;
    
    for (int c=0;c<cols;++c){
        if(matrix[0][c]==1) wallSeen=true;
        dp[0][c]=(!wallSeen) ? 1 : 0;
    }
    print_utils::printBoxedLabel("dp first row");
    rowByRowPrint(dp);
    bool wallSeenCol = false;
    for (int r = 0; r < rows; ++r) {
        if (matrix[r][0] == 1) wallSeenCol = true;
        dp[r][0] = (!wallSeenCol) ? 1 : 0;
    print_utils::printBoxedLabel("dp first col");
    rowByRowPrint(dp);
    }
    
    return 5;
}
