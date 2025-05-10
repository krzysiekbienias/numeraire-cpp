#pragma once

// Declare your transponse_matrix interface here.
void transponseSquareMatrix(std::vector<std::vector<int>>& matrix);
// Since transposeRectangleMatrix does not modify the input matrix, I take it by const& to avoid unnecessary copying while guaranteeing that the input will not be modified.

std::vector<std::vector<int>> transposeRectangleMatrix(const std::vector<std::vector<int>>& matrix)
