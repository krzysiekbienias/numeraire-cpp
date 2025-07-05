#include "unordered_set"

// Implement your first_duplicated_value logic here.
int firstDuplicateValue(const std::vector<int>& array){
    std::unordered_set<int> seen;
    for (int num:array){
        if (seen.count(num)){
            return num;
        }
        else{
            seen.insert(num);
        }        
    }
    return -1;
    
}
