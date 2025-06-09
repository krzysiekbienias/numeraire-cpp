#pragma once

// Declare your parmutations interface here.
#include <vector>
#include <unordered_set>
std::vector<std::vector<int>> permutations (std::vector<int> nums);
void backtrack(const std::vector<int>& nums,std::vector<int>& candidate,  std::unordered_set<int>& used, std::vector<std::vector<int>>& results);
