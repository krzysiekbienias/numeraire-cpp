#include "math_and_geometry/handshake.hpp"

// Implement your handshake logic here.

//calulate how many 2 elements subsets we have from n elements set
int handshake(int n){
    return n*(n-1)/2;
}
