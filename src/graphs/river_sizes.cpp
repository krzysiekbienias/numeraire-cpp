#include<vector>
#include "graphs/river_sizes.hpp"
#include "matrix/is_within_bounds.hpp"

// Implement your river_sizes logic here.
std::vector<int> riverSizes(std::vector<std::vector<int>>& matrix){
    std::vector<int> results;
    for (int r=0;r<matrix.size();++r){
        for(int c=0;c<matrix[0].size();++c){
            if (matrix[r][c]==1){
                int riverSize=dfsRiverHelper(r,c,matrix);
                results.push_back(riverSize);
            }
        }
    }
    
    return results;
}

int dfsRiverHelper(int r, int c, std::vector<std::vector<int>>& matrix){
    int riverSize=1;
    matrix[r][c]=-1;//visited
    std::vector<std::pair<int, int>> directions={{-1,0},{1,0},{0,-1},{0,1}};
    for (auto & [dr,dc]:directions){
        int newRow=r+dr;
        int newCol=c+dc;
        if (isWithinBounds(newRow, newCol, matrix) && matrix[newRow][newCol]==1) {
            riverSize+=dfsRiverHelper(newRow, newCol, matrix);
        }
    }
    return riverSize;
}
