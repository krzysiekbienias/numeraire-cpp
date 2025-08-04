#include <vector>
#include <string>
#include <unordered_map>

// Implement your group_anagrams logic here.
std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string> words){
    std::vector<std::vector<std::string>> result;
    std::unordered_map<std::string, std::vector<std::string>> anagramGroups;
    for (std::string word:words){
        std::string sortedWord=word;
        std::sort(sortedWord.begin(), sortedWord.end());
        auto it=anagramGroups.find(sortedWord);
        if (it!=anagramGroups.end()){
            //key exists
            it->second.push_back(word);
        }
        else{
            //key doesn't exist yet - create new group
            anagramGroups[sortedWord]={word};
        }
    }
    for (const auto& [key,group]:anagramGroups){
        result.push_back(group);
    }
    return result;
}
