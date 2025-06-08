#include <vector>

// Implement your zero_cross logic here.
void zeroCross(std::vector<std::vector<int>>& matrix){
    if (matrix.empty()) return;
    int r=matrix.size();
    int c=matrix[0].size();
    
    std::vector<bool> row (r,false);
    std::vector<bool> col (c,false);
    
    //first pass: identify rows and columns to be zeroed
    for (int i=0;i<r;++i){
        for (int j=0;j<c;++j){
            if (matrix[i][j]==0){
                row[i]=true;
                col[j]=true;
            }
        }
    }
    for (int i=0;i<r;++i){
        for (int j=0;j<c;++j){
            if (row[i]==0 || col[j]==0){
                matrix[i][j]=0;
            }
        }
    }
}


