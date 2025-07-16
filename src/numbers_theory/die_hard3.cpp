#include <string>
#include "numbers_theory/die_hard3.hpp"
#include <numeric>

// Implement your die_hard3 logic here.
std::string dieHard3(int a, int b, int c){
    int gcd=std::gcd(a, b);
    if (c<=std::max(a, b) && c%gcd==0) {
        return "YES";
    }
    else return "NO";
}
