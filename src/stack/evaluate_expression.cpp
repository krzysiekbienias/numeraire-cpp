#include "stack/evaluate_expression.hpp"
#include <vector>
#include <string>
#include <cctype> // character clasification functions, required for isdigit()

// as of now not parenthesis
int evaluateExpression(const std::string & expression){
    int result(0);
    int sign(1);
    int currentNumber(0);
    for(char ch : expression){
        if(std::isdigit(ch)){
            currentNumber=currentNumber*10+(ch-'0');
            
        }
        else if (ch=='+' || ch=='-'){
            result+=currentNumber*sign;
            sign= (ch=='+')? 1 : -1;
            currentNumber=0; // reset current number if we encounter character that is not a digit
        }
    }
    return result+currentNumber*sign;
    
}
