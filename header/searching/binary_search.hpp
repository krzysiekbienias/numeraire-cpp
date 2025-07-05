#pragma once

/**
 * @brief Performs binary search on a sorted vector to find the target value.
 *
 * Searches the sorted vector `array` for the specified `target` using binary search.
 * Returns the index of `target` if found; otherwise, returns -1.
 *
 * This implementation avoids integer overflow by computing the midpoint as:
 * `mid = left + (right - left) / 2`
 *
 * @param array A sorted vector of integers to search.
 * @param target The integer value to search for in the vector.
 * @return Index of the target if found; otherwise, -1.
 */

int binary_search(const std::vector<int>& array,int target);
