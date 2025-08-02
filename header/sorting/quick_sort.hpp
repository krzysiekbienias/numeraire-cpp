#pragma once
#include <vector>


int partition(std::vector<int>& arr);

void quickSortHelper(std::vector<int>& arr,int left,int right);

std::vector<int> quickSort(std::vector<int>&arr);
