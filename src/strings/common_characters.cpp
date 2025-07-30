#include <vector>
#include <string>
#include <unordered_set>

// Implement your common_characters logic here.
std::vector<std::string> commonCharacter(std::vector<std::string >words){
    std ::vector<std::string> result;
    std::unordered_set<char> charSet(words[0].begin(),words[0].end());
    for (int i=1;i<words.size();++i){
        std::unordered_set<char> tempSet(words[i].begin(),words[i].end());
        std::unordered_set<char> intersection;
        for (char ch:tempSet){
            if (charSet.count(ch)) {
                intersection.insert(ch);
            }
        }
        charSet=std::move(intersection);
    }
    for (char ch : charSet){
        result.push_back(std::string(1,ch));
        
    }
    return result;
}
