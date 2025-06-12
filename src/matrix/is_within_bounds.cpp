#include "matrix/is_within_bounds.hpp"

// Implement your is_within_bounds logic here.
bool isWithinBounds(int r, int c, std::vector<std::vector<int>>& matrix){
    int nbRows=matrix.size();
    int nbCols=matrix[0].size();
    if (r>=0 && r<nbRows && c>=0 &&c<nbCols) {
        return true;
    }
    return false;
}
