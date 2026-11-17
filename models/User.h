#pragma once
#include <string>

class User{
    private:
        std::string username;
        std::string hash;
        std::string salt;

    public:
        User();
        User(std::string username, std::string password);
        std::string getUserName(){return this->username;};
        std::string getHash(){return this->hash;};
        std::string getSalt(){return this->salt;};
};
