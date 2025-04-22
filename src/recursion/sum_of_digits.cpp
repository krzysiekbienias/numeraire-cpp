

// Implement your sum_of_digits logic here.
int sumOfDigits(int n){
    if (n<=9) return n;
    else{
        return n%10+sumOfDigits(n/10);
    }
}
