#include "User.h"


User::User(std::string &username, std::string &password)
    :username(username)
{
    this->salt = Hasher::genSalt();
    this->hash = Hasher::hash(password, this->salt);
}

User::User(std::string &username, std::string &password, Salt &salt)
    :username(username)
    ,salt(salt)

{
    this->hash = Hasher::hash(password, salt);
}

User::User(std::string &username, Hash &hash, Salt &salt)
    :username(username)
    ,hash(hash)
    ,salt(salt)
{ }
