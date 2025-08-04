#pragma once
#include<vector>
// Declare your longest_increasing_path interface here.
int longestIncreasingPath(const std::vector<std::vector<int>>& matrix);
int dfs(int r, int c, const std::vector<std::vector<int>>& matrix,std::vector<std::vector<int>>& dp);
