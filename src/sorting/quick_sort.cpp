
#include <vector>
#include "sorting/quick_sort.hpp"

//function to find pivot index
int partition(std::vector<int>& arr,int left, int right){
    int pivot=arr[right];  //We chose pivot in start way as a classic approach pivot is a value NOT index
    int i=left;
    for (int j=left;j<right;++j){
        if (arr[j]<pivot){
            std::swap(arr[i], arr[j]);
            i++;
        }
    }
    //After FOR loop
    std::swap(arr[i],arr[right]); // put pivot in correct place
    
    return i;
}

void quickSortHelper(std::vector<int>& arr,int left,int right){
    if (left<right) {
        int pivotIndex=partition(arr,left,right);
        quickSortHelper(arr, left, pivotIndex-1);   // Sort left half
        quickSortHelper(arr, pivotIndex+1, right);   //Sort right half
    }
    
}

//Convinience wrapper

std::vector<int> quickSort(std::vector<int>&arr){
    if (arr.empty()) return {};
    quickSortHelper(arr, 0, arr.size()-1);
    return arr;
}

