#include<vector>


// Implement your sorted_squared logic here.
std::vector<int> sortedSquared(const std::vector<int>& array){
    std::vector<int> sortedArray;
    sortedArray.reserve(array.size());
    for (int num : array){
        sortedArray.push_back(num*num);
    }
    std::sort(sortedArray.begin(),sortedArray.end());
    return sortedArray;
}



std::vector<int> sortedSquaredFaster(const std::vector<int>& array){
    int n =array.size();
    std::vector<int> sortedSquared(n);
    int left=0;
    int right=n-1;
    int pos=n-1;//we traverse from back(right)
    while (left<=right){
        int leftVal=array[left];
        int rightVal=array[right];
        if (abs(leftVal)>abs(rightVal)){
            sortedSquared[pos--]=leftVal*leftVal;
            ++left;
        }
        else{
            sortedSquared[pos--]=rightVal*rightVal;
            --right;
        }
    }
    return sortedSquared;
}
