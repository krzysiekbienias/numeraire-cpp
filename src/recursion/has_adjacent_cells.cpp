

// Implement your has_adjacent_cells logic here.
#include "iostream"

bool hasAdjacentCells(std::string & text, size_t i) {
    if (i>=text.length()) {
        return false;
    }
    if (text[i]==text[i-1]) {
        return true;
    }
    return hasAdjacentCells(text,i+1);

}
