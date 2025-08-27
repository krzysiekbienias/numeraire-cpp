#include "sorting/bubble_sort.hpp"
#include <iostream>


// Implement your bubble_sort logic here.
std::vector<int> bubbleSort(std::vector<int>& array){
    int n=array.size();
    int totalPasses=0; //helper to measure small optimisation
    int savedPasses=0;
    for (int j=n-1; j>0;--j){//<------ we shring from the end
        bool isSorted=true; //optymistic
        totalPasses++;
        for (int i=0;i<j;++i){
            if (array[i]>array[i+1]) {
                std::swap(array[i], array[i+1]);
                isSorted=false; //out of order pair has been found;
            }
        }
        if(isSorted){
            savedPasses=j;
            std::cout<<"[Optimized] Early exit after"<<totalPasses<<" passes. Saved "<<savedPasses<<" unnecessary passes.\n";//DEBUG
            break;
            
        }
    }
    if(savedPasses==0){
        std::cout << "[Full Run] No early exit. Completed " << totalPasses << " passes.\n";// DEBUG
    }
    return array;
}
