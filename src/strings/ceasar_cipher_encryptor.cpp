#include <string>
#include "strings/ceasar_cipher_encryptor.hpp"

 //Implement your ceasar_cipher_encryptor logic here.
std::string ceasarCypherEncryptor(std::string order,int k){
    std::string encryptedOrder ="";
    k%=26;
    for (char ch:order){
        if (static_cast<int>(ch)+k<=122) {
            int shiftedASCII=static_cast<int>(ch)+k;
            encryptedOrder+=static_cast<char>(shiftedASCII);
        }
        else{
            int shiftedASCII=(static_cast<int>(ch)+k)%122;
            encryptedOrder+=static_cast<char>(96+shiftedASCII);
        }
    }
    return encryptedOrder;
}
