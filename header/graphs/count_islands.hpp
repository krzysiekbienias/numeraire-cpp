#pragma once
#include <vector>

// Declare your count_islands interface here.

int countIslands(std::vector<std::vector<int>>& matrix);


void dfs(int r, int c, std::vector<std::vector<int>>& matrix);

