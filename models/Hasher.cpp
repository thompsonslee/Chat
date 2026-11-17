#include "Hasher.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

std::array<unsigned char, crypto_generichash_BYTES>* Hasher::hash(std::string &password){

    std::array<unsigned char*, crypto_generichash_BYTES> salt;

    std::fstream stream("/dev/urandom");
    stream.read(reinterpret_cast<char*>(&salt), 32);

    if(sodium_init() < 0){
        throw new std::runtime_error("failed to initialize sodium");
    }

    auto hash = new std::array<unsigned char, crypto_generichash_BYTES>;
    
    std::string saltString = reinterpret_cast<const char*>(salt.data());
    std::string pwdSalt = password + saltString;

    if(crypto_generichash(
            hash->data(), 
            crypto_generichash_BYTES, 
            reinterpret_cast<const unsigned char*>(pwdSalt.data()), 
            password.size(), 
            NULL, 
            0) 
     != 0){
        throw new std::runtime_error("failed to hash");
    }

    return hash;
}

void Hasher::test(){
    std::string pw = "test1";
    std::string pw2 = "differentPW";

    Hash *pwHash = Hasher::hash(pw);
    Hash *pwHash2 = Hasher::hash(pw);
    Hash *pwHash3 = Hasher::hash(pw2);

    if(*pwHash != *pwHash2){
        throw new std::runtime_error("pw's dont match");
    } else {
        std::cout << "test1 passed" << std::endl;
    }

    if(*pwHash == *pwHash3){
        throw new std::runtime_error("pw's match");
    } else {
        std::cout << "test1 passed" << std::endl;
    }

}





