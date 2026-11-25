#include "Hasher.h"
#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

Hash Hasher::hash(const std::string &password, const Salt &salt){

    if(sodium_init() < 0){
        throw std::runtime_error("failed to initialize sodium");
    }

    Hash hash;
    
    std::string saltString(reinterpret_cast<const char*>(salt.data()), salt.size());
    std::string pwdSalt = password + saltString;

    if(crypto_generichash(
            hash.data(), 
            crypto_generichash_BYTES, 
            reinterpret_cast<const unsigned char*>(pwdSalt.data()), 
            pwdSalt.size(), 
            NULL, 
            0) 
     != 0){
        throw std::runtime_error("failed to hash");
    }

    return hash;
}

void Hasher::test(){
    std::string pw = "test1";
    std::string pw2 = "differentPW";
    
    Salt salt1 = Hasher::genSalt();
    Salt salt2 = Hasher::genSalt();
    Salt salt3 = Hasher::genSalt();

    Hash pwHash = Hasher::hash(pw, salt1);
    Hash pwHash2 = Hasher::hash(pw, salt1);
    Hash pwHash3 = Hasher::hash(pw2, salt2);
   
    try{
        if(salt1 == salt2){
            throw new std::runtime_error("separate generated salts are identical");
        } else {
            std::cout << "test1 passed" << std::endl;
        }

        if(pwHash != pwHash2){
            throw new std::runtime_error("pw's dont match");
        } else {
            std::cout << "test2 passed" << std::endl;
        }

        if(pwHash == pwHash3){
            throw new std::runtime_error("pw's match");
        } else {
            std::cout << "test3 passed" << std::endl;
        }
    } catch (std::runtime_error *e){
        std::cout << e->what() << std::endl;
    }
}

Salt Hasher::genSalt(){
    Salt salt;

    randombytes_buf(salt.data(), salt.size());

    return salt;
}


