#include "matrix/row_by_row.hpp"
#include<vector>
#include<iostream>
// Implement your row_by_row logic here.

std::vector<int> rowByRow (const std::vector <std::vector<int>>& matrix){
    std::vector<int> results;
    int rows=matrix.size();
    int cols=matrix[0].size();
    
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            results.push_back(matrix[i][j]);
        }
    }
    return results;
}


void rowByRowPrint (const std::vector<std::vector<int>>& matrix){
    int rows=matrix.size();
    int cols=matrix[0].size();
    
    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}
