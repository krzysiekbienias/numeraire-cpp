#include <vector>
#include <string>
#include "recursion/phone_number_mnemonics.hpp"


void backtrack(int index, const std::string & digits, const std::vector<std::string>& mapping,
               std::string& current, std::vector<std::string> result){
    if(index==digits.size()){
        result.push_back(current);
        return;
    }
    int digit =digits[index]-'0';
    for (char c:mapping[digit]){
        current.push_back(c);
        backtrack(index+1, digits, mapping, current, result);
        current.pop_back();
    }
}

// Declare your phone_number_mnemonics interface here.
std::vector<std::string> phoneNumberMnemonics(std::string phoneNumber){
    //edge case
    std::vector<std::string> mapping={
        "0",
        "1",
        "abc",
        "def",
        "ghi",
        "jkl",
        "mno",
        "pqrs",
        "tuv",
        "wxyz"
    };
    
    std::vector<std::string> result;
    std::string current;
    // kick off parameters
    backtrack(0, phoneNumber, mapping, current, result);
    return result;
    
    
}
