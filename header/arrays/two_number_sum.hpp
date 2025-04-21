#pragma once
#include <vector>

/**
 * @brief Find two numbers in the array that sum up to the target sum.
 *
 * This function searches for a pair of numbers in the provided array that
 * add up to the specified target sum. It returns the first found pair
 * as a vector. If no such pair exists, it returns an empty vector.
 *
 * @param array A vector of integers in which to search for the two numbers.
 * @param targetSum The target sum that the two numbers should equal.
 * @return std::vector<int> A vector containing the two numbers that sum to the target sum.
 */
std::vector<int> twoNumberSum(std::vector<int>& array, int targetSum);

