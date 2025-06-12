#pragma once

// Declare your river_sizes interface here.



bool isWithinBounds(int r, int c, std::vector<std::vector<int>>& matrix);
int dfsRiverHelper(int r, int c, std::vector<std::vector<int>>& matrix);


std::vector<int> riverSizes(std::vector<std::vector<int>>& matrix);
