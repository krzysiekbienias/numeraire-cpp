#include "bit_manipulation/reverse_bits.hpp"
#include <cstdint>
#include <iostream>

// Implement your reverse_bits logic here.
uint32_t reverseBits(uint32_t n){
    uint32_t result=0;
    for (int i=0;i<32;++i){
        int bit=n&1;
        result<<=1;
        result|=(bit);
        n>>=1;
        std::cout << "Step " << i + 1 << ": "
                      << "bit = " << bit
                      << ", result = " << std::bitset<32>(result)
                      << ", n = " << std::bitset<32>(n) << "\n";
    }
    return result;
}
