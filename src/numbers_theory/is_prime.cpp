#include <cmath>
#include "numbers_theory/is_prime.hpp"

// Implement your is_prime logic here.
bool isPrime(int num){
    if (num<=1) {
        return false;
    }
    for (int i=2;i<=std::sqrt(num);++i){
        if(num%i==0){
            return false;
        }
    }
    return true;
}
