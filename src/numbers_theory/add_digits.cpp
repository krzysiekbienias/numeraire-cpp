#include "numbers_theory/add_digits.hpp"

// Implement your add_digits logic here.
int addDigits(int num){
    while (num>=10){
        int sum=0;
        while (num>0) {
            sum+=num%10;
            num/=10;
        }
        num=sum;
    }
    return num;
};


int addDigitsMathTrick(int num){
    if (num==0) return 0;
    return num%9==0 ? 9 : num%9;
}
