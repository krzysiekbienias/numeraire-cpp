#include <unordered_map>

// Implement your nth_fibbonaci logic here.
int nthFibbonaci(int n,std::unordered_map<int,int>& lookup){
    if(lookup.count(n)) return lookup[n];
    
    if (n==0) return 0;
    if (n==1) return 1;
    
    lookup[n]=nthFibbonaci(n-1, lookup)+nthFibbonaci(n-2, lookup);
    return lookup[n];
}
