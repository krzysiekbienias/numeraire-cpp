#include "graphs/count_islands.hpp"
#include "matrix/is_within_bounds.hpp"

// Implement your count_islands logic here.
int countIslands(std::vector<std::vector<int>>& matrix){
    int count =0;
    for (int r=0;r<matrix.size();++r){
        for(int c=0;c<matrix[0].size();++c){
            if (matrix[r][c]==1){
                dfs(r,c,matrix);
                count++;
            }
        }
    }
    
    return count;
}



void dfs(int r, int c, std::vector<std::vector<int>>& matrix){
    matrix[r][c]=-1;//visited
    std::vector<std::pair<int, int>> directions={{-1,0},{1,0},{0,-1},{0,1}};
    for (auto & [dr,dc]:directions){
        int newRow=r+dr;
        int newCol=c+dc;
        if (isWithinBounds(newRow, newCol, matrix) && matrix[newRow][newCol]==1) {
            dfs(newRow, newCol, matrix);
        }
    }
}
