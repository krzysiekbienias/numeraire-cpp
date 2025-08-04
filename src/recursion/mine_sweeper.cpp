#include<vector>
#include<string>
#include"recursion/mine_sweeper.hpp"

//bool isWithinBounds(int r, int c, std::vector<std::vector<char>>& matrix){
//    int nbRows=matrix.size();
//    int nbCols=matrix[0].size();
//    if (r>=0 && r<nbRows && c>=0 &&c<nbCols) {
//        return true;
//    }
//    return false;
//}
//
//
//void dfs(int r, int c, std::vector<std::vector<char>>& matrix){
//    std::vector<std::pair<int, int>> directions={{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};
//    for (auto & [dr,dc]:directions){
//        int newRow=r+dr;
//        int newCol=c+dc;
//        if (isWithinBounds(newRow, newCol, matrix) && matrix[newRow][newCol]=='H') {
//            if
//            dfs(newRow, newCol, matrix);
//        }
//    }
//}
//
//
//// Implement your mine_sweeper logic here.
//std::vector<std::vector<std::string>> revealMinesweeper(std::vector<std::vector<std::string>> board, int row, int column){
//    if (isWithinBounds(row, column, board) && board[row][column]=='M') {
//        board[row][column]='X';
//        return board;
//    }
//   else:
//       dfs(row, column, board);
//    return board;
//}
