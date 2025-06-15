#pragma once
#include <vector>

// Declare your island_perimeter interface here.

bool isWithinBounds(int r, int c, const std::vector<std::vector<int>>& matrix);
void dfs(int r, int c, std::vector<std::vector<int>>& matrix,int& edges);
int islandPerimeter(std::vector<std::vector<int>>& grid);




