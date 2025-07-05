#include "searching/index_equal_value.hpp"
#include <vector>

// Implement your index_equal_value logic here.
int indexEqualsValue(const std::vector<int>& array){
    for (int i=0;i<array.size();++i){
        if(i==array[i]){
            return i;
        }
        
    }
    return -1;
    
}
