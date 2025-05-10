# include <iostream>
# include "unordered_set"
# include "stack"

// Implement your balanced_brackets logic here.
bool balancedBracket(std::string& str){
    std::unordered_set<char> setOfBrackets{'(',')','[',']','{','}'};
    std::unordered_map<char, char> bracketsMap{{'(',')'},{'[',']'},{'{','}'}};
    std::unordered_set<char> openBrackets{'(','[','{'};
    std::stack<char> bracketsStack;
    for (char ch:str){
        if (!setOfBrackets.count(ch)){
            continue;
        }
        if (openBrackets.find(ch)!=openBrackets.end()){
            bracketsStack.push(ch);
        }// order of condition MATTERS
        else if (!bracketsStack.empty() && ch==bracketsMap[bracketsStack.top()]){
            bracketsStack.pop();
                
        }
        else return false;
        }
        
    return bracketsStack.size()==0;
    
}

