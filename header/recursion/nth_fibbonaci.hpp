#pragma once
#include <unordered_map>

// Declare your nth_fibbonaci interface here.
int nthFibbonaci(int n);
int nthFibbonaci(int n, std::unordered_map<int, int>& memo);
