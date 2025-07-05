#include <vector>
#include <iostream>

// Implement your binary_search logic here.
int binary_search(const std::vector<int>& array,int target){
    if (array.empty()) return -1;
    int left=0;
    int right=array.size()-1;
    
    while (left<=right) {
        
        int mid=left+(right-left)/2;
        if (array[mid]==target) return mid;
        else if (array[mid]<target){
            left=mid+1;
        }
        else{
            right=mid-1;
        }
        
    }
    return -1;
}
