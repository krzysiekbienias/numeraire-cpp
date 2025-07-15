#include <vector>
#include <unordered_map>
#include "arrays/lonely_number.hpp"
#include "utils/print_utils_templates.hpp"

int lonelyNumber(const std::vector<int> & numbers)
{
    std::unordered_map<int,int> frequenceContainer;
    for(int num : numbers){
        frequenceContainer[num]++;
        
    }
    print_utils::printUnorderedMap(frequenceContainer);
    
    for (auto &[key,value]:frequenceContainer){
        if (value==1){
            return key;
        }
        
    }
    return 0;
    
}

//NOTE

//we may use bit manpul;ation to achive this in O(1) space complexity.
