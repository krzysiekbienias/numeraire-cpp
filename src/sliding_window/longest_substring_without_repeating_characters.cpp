#include "unordered_map"
#include "string"

// Implement your longest_substring_without_repeating_characters logic here.

int longestStringWithoutRepeatingCharacters(const std::string& str){
    std::unordered_map<char, int> charCounter;
    int left=0; //start of current window
    int right =0;//iterate through string
    int result=0;
    while (right<str.length()){
        char r = str[right];
        charCounter[r]++;
        while (charCounter[r]>1) { //we shring the window if we see that for a character we already saw it.
            char l =str[left];
            charCounter[l]--;
            left++;
            
        }
        result=std::max(result,right-left+1);
        right++;
    }
    return result;
}


// more natural it is easier to implement this with hash set .
