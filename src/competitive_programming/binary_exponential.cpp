#include "competitive_programming/binary_exponential.hpp"

// Implement your binary_exponential logic here.
long long binExponentialWithMod(long long a,long long b, long long m){
    a%=m; //<--- extra safety step‼️
    long long res=1;
    while (b>0) {
        if(b&1){
            res=res*a%m; //<--- 1 on bit contribute to result
        }
        a=a*a%m;
        b>>=1;//<---- we move right and update 🤓 not b>>1 but b>>=1
    }
    return res;
}

long long binExponential(long long a,long long b){
    long long res=1;
    while (b>0) {
        if(b&1){
            res=res*a; //<--- 1 on bit contribute to result
        }
        a=a*a;
        b>>=1;//<---- we move right and update 🤓 not b>>1 but b>>=1
    }
    return res;
}

long long modInverse(long long a, long long p){
    return binExponentialWithMod(p, p-2, p);
}

