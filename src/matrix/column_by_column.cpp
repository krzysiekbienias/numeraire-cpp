#include "matrix/column_by_column.hpp"
#include<vector>

// Implement your column_by_column logic here.
std::vector<int> colByCol (const std::vector <std::vector<int>>& matrix){
    std::vector<int> results;
    int rows=matrix.size();
    int cols = matrix[0].size();
    
    for (int i=0; i<cols; ++i) {
        for (int j=0; j<rows; ++j) {
            results.push_back(matrix[j][i]);
        }
    }
    return results;
}
