#pragma once


//Since I plan to modify the input strings by sorting them, I’ll pass them by non-const reference to avoid copying. If I were not modifying them, I would use const std::string&.
bool isPermutation(std::string& s1, std::string& s2);
bool isPermuatationBetterTimeComplexity(std::string& s1, std::string& s2);
