#include<vector>


// Implement your insertion_sort logic here.
std::vector<int> insertionSort(std::vector<int>& arr){
    for (int i=1;i<arr.size();++i){
        int j=i;
        while (j>0&&arr[j]<arr[j-1]) {
            std::swap(arr[j], arr[j-1]);
            j--;
        }
    }
    return arr;
}
