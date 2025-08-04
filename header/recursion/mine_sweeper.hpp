#pragma once

#include<vector>
#include<string>
bool isWithinBounds(int r, int c, std::vector<std::vector<char>>& matrix);
void dfs(int r, int c, std::vector<std::vector<char>>& matrix);

std::vector<std::vector<std::string>> revealMinesweeper(std::vector<std::vector<std::string>> board, int row, int column);
