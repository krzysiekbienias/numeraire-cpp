#pragma once

// Declare your has_adjacent_cells interface here.
#include <string>

/**
 * @brief Recursively checks if a string contains adjacent identical characters.
 *
 * @param str The input string to check.
 * @param i The current index in the recursion (default is 0).
 * @return true if the string contains any adjacent identical characters, false otherwise.
 *
 * @note The check starts from index 0 and compares each character with the next one.
 */
bool hasAdjacentCells(std::string & str, size_t i = 1);
