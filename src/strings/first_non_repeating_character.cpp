#include <string>
#include <unordered_map>
#include "utils/print_utils_templates.hpp"

// Implement your first_non_repeating_character logic here.
int firstNonRepeatingCharacter(std::string str){
    std::unordered_map<char, int> letterFrequency;
    for (char ch:str){
        letterFrequency[ch]++;
    }
    
    for (int i=0;i<str.size();++i){
        std::cout<<str[i]<<'\n';
        if(letterFrequency[str[i]]==1){
            return i;
        }
    }
    return -1;
}
