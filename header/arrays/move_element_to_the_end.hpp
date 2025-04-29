#pragma once

// Declare your move_element_to_the_end interface here.
std::vector<int> moveElementToTheEnd(const std::vector<int>& array ,int toMove);


// Since the task requires in-place modification, I pass the array by non-const reference.
// The function returns void because all changes are made directly to the provided array.
// I accept toMove by value because it’s a simple integer.
void moveElementToTheEndInPlace(std::vector<int>& array ,int toMove)
