#pragma once
#include "../models/User.h"
#include "../Database.h"
#include <string>


class UserRepo{
    public:
        UserRepo(Database *db) : db(db){}
        std::optional<User> getUser(std::string &username);
        std::optional<User> getUser(std::string &username, std::string &password);
        void createUser(User &user);
        void createUser(const std::string &username, const Hash &hash, const Salt &salt);
        void testUserRepo();
    private:
        Database *db;
};
