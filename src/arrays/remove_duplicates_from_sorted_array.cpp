# pragma once
#include<vector>

// Implement your remove_duplicates_from_sorted_array logic here.
int removeDuplicatesFromSortedArray(std::vector<int>& arr){
    int insertIndex{1};
    for (int i=1;i<arr.size();++i){
        //we skip to next index if we see a duplicate element, not enter to the if loop and go to another i
        if(arr[i]!=arr[i-1]){
            arr[insertIndex]=arr[i];
            insertIndex++;
        }
    }
    return insertIndex;

}
