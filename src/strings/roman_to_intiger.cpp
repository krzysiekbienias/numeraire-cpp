#include <string>
#include <unordered_map>
#include <iostream>
// Implement your roman_to_intiger logic here.
int romanToIntiger(std::string roman){
    //edge case
    if (roman.size()==0) return 0;
    std::unordered_map<char , int> romanIntMap {{'M',1000},
        {'D',500},
        {'C',100},
        {'L',50},
        {'X',10},
        {'V',5},
        {'I',1}};
    int arabicNumber=0;
    for (int i=0; i<roman.size()-1;++i){
        if(romanIntMap[roman[i]]>=romanIntMap[roman[i+1]]){
            arabicNumber+=romanIntMap[roman[i]];
        }
        else{
            arabicNumber-=romanIntMap[roman[i]];
        }
    }
    arabicNumber+=romanIntMap[roman.back()];
    return arabicNumber;
}
