#include<vector>
#include "graphs/island_perimeter.hpp"
#include "matrix/is_within_bounds.hpp"

// Implement your island_perimeter logic here.
int islandPerimeter(std::vector<std::vector<int>>& matrix){
    int edges=0; //we will modify this by referecne from dfs
    for (int r=0;r<matrix.size();++r){
        for(int c=0;c<matrix[0].size();++c){
            if (matrix[r][c]==1){
                dfs(r,c,matrix,edges);//start traversal
                return edges;
            }
        }
    }
    
    return 0;
}


void dfs(int r, int c, std::vector<std::vector<int>>& matrix , int & edges){
    if (matrix[r][c] != 1) return;  //already visited or water
    matrix[r][c]=-1;//visited
    std::vector<std::pair<int, int>> directions={{-1,0},{1,0},{0,-1},{0,1}};
    for (auto & [dr,dc]:directions){
        int newRow=r+dr;
        int newCol=c+dc;
        if (!isWithinBounds(newRow, newCol, matrix) || matrix[newRow][newCol]==0) {
            edges++;
        }
        else if(matrix[newRow][newCol]==1){
            dfs(newRow, newCol, matrix,edges);
        }
            
    }
}
