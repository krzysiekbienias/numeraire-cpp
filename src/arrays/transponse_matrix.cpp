#include <vector>

// Implement your transponse_matrix logic here.

void transponseSquareMatrix(std::vector<std::vector<int>>& matrix){
    int r=matrix.size();
    int c=matrix[0].size();
    if(r==c){
        for (int i=0;i<r;++i){
            for (int j=i+1;j<c;++j){
                std::swap(matrix[i][j],matrix[j][i]);
            }
        }
        
    }
    
}

std::vector<std::vector<int>> transposeRectangleMatrix(const std::vector<std::vector<int>>& matrix){
    if(matrix.empty()||matrix[0].empty()) return {};
    int rows=matrix.size();
    int cols=matrix[0].size();
    std::vector<std::vector<int>> transposedMatrixContainer(cols,std::vector<int>(rows,0));
    for (int i=0;i<rows;++i){
        for (int j=0;j<cols;++j){
            transposedMatrixContainer[j][i]=matrix[i][j];
        }
    }
    return transposedMatrixContainer;
    
}
