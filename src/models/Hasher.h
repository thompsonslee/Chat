#pragma once
#include <sodium.h>
#include <string>
#include <array>

using Hash = std::array<unsigned char, crypto_generichash_BYTES> ;
using Salt = std::array<unsigned char*, crypto_generichash_BYTES> ;

class Hasher{
    public:
        static Hash hash(const std::string &password, const Salt &salt) ;
        static Salt genSalt() ;
        static void test() ;
};
