#pragma once
#include <string>
#include "Hasher.h"

class User{
    private:
        std::string username;
        Hash hash;
        Salt salt;

    public:
        User();
        User(std::string &username, std::string &password);
        User(std::string &username, std::string &password, Salt &salt);
        User(std::string &username, Hash &hash, Salt &salt);
        std::string getUserName(){return this->username;};
        Hash getHash(){return this->hash;};
        Salt getSalt(){return this->salt;};
};
