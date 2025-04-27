#include <string>
#include <algorithm>
#include <unordered_map>
#include<iostream>

// Implement your is_permutation logic here.
bool isPermutation(std::string& s1, std::string& s2){
    if (s1.length()!=s2.length()) return false;
    std::sort(s1.begin(),s1.end());
    std::sort(s2.begin(),s2.end());
    
    return s1==s2;
}


bool isPermuatationBetterTimeComplexity(std::string& s1, std::string& s2){
    if (s1.length()!=s2.length()) return false;
    // I’ll traverse s1 and build a frequency map where the key is the character and the value is the count of that character. I increment s1Map[ch]++ each time I see a character.
    std::unordered_map<char,int> s1Map;
    std::unordered_map<char,int> s2Map;
    for (auto ch:s1){
        s1Map[ch]++;
    }
    for (auto ch :s2){
        s2Map[ch]++;
    }
    return s1Map==s2Map;
}
    
