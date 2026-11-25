#include "UserRepo.h"
#include <optional>
#include <stdexcept>
#include <string>
#include <iostream>

std::optional<User> UserRepo::getUser(std::string &username){

    DatabaseQuery dbQuery(
            db->getDbConnection(), 
            R"(
SELECT username,hash,salt
FROM users
WHERE username = ?
    )");

    dbQuery.bind(1,username);
    dbQuery.step();

    if(dbQuery.getResultsCount() == 0){
        return std::nullopt;
    }

    std::optional<Hash> hash = dbQuery.getHash(1);
    std::optional<Salt> salt = dbQuery.getSalt(2);
    
    if(hash == std::nullopt){
        throw new std::runtime_error("hash provided by dbQuery is null");
    };
    if(salt == std::nullopt){
        throw new std::runtime_error("salt provided by dbQuery is null");
    }

    return User(
        username, 
        hash.value(),
        salt.value()
        );
};

std::optional<User> UserRepo::getUser(std::string &username, std::string &password){
    
    std::optional<User>user = UserRepo::getUser(username);

    if(!user){
        return user;
    }
    
    std::cout << "found user" << std::endl;

    Hash newHash = Hasher::hash(password, user->getSalt());
    Hash currentHash = user->getHash();
    if(newHash != currentHash){

    std::cout << "hash doesnt match" << std::endl;
        return std::nullopt;
    };

    return user.value();
}

void UserRepo::createUser(User &user){
    DatabaseQuery dbQuery(
            db->getDbConnection(),
        R"(
INSERT INTO users 
(username, hash, salt)
VALUES(?,?,?)
        )"

        );
    std::string username = user.getUserName();
    Hash hash = user.getHash();
    Salt salt = user.getSalt();

    dbQuery.bind(1, username);
    dbQuery.bindblob(2,&hash, 32);
    dbQuery.bindblob(3,&salt, 32);
    
    dbQuery.step();
}

void UserRepo::createUser(const std::string &username, const Hash &hash, const Salt &salt){
    DatabaseQuery dbQuery(
            db->getDbConnection(),
        R"(
INSERT INTO users 
(username, hash, salt)
VALUES(?,?,?)
        )"
        );

    dbQuery.bind(1, username);
    dbQuery.bindblob(2,&hash, 32);
    dbQuery.bindblob(3,&salt, 32);
    
    dbQuery.step();

}
void UserRepo::testUserRepo(){
    UserRepo userRepo(db);

    std::string username1 = "user1";
    std::string testPassword1 = "testpassword1";
    User user1(username1, testPassword1);

    std::string username2 = "user2";
    std::string testPassword2 = "supersecretpassWORD";
    User user2(username2, testPassword2);

    
    std::string bobsUserName = "bob";
    std::string bobsPassword = "bobsPASSword";
    User bob(bobsUserName, bobsPassword);

    userRepo.createUser(user1);
    userRepo.createUser(user2);

    Salt bobsSalt = Hasher::genSalt();
    Hash bobsHash = Hasher::hash(bobsPassword, bobsSalt);
    Hash bobsHash2 = Hasher::hash(bobsPassword, bobsSalt);
    userRepo.createUser(bobsUserName, bobsHash, bobsSalt);

    std::optional<User> getBobBack = userRepo.getUser(bobsUserName);


    std::optional<User> getUser1 = userRepo.getUser(username1, testPassword1);
    std::optional<User> getUser2 = userRepo.getUser(username2, testPassword2);
    std::optional<User> getBob = userRepo.getUser(bobsUserName, bobsPassword);
    
    std::optional<User> shouldBeNull = userRepo.getUser(username1, testPassword2);
    std::optional<User> shouldBeNull2 = userRepo.getUser(bobsUserName, testPassword1);

    std::vector<bool> tests;
    
    tests.push_back(bobsHash == bobsHash2);
    tests.push_back(getBobBack.has_value());
    tests.push_back(getBobBack.value().getHash() == bobsHash);
    std::cout << "bobs hash before: " << bobsHash.data()[0] << std::endl;
    std::cout << "bobs hash after: " << getBobBack.value().getHash().data()[0] << std::endl;

    tests.push_back(getUser1.has_value());
    tests.push_back(getUser2.has_value());
    tests.push_back(getBob.has_value());
    
    tests.push_back(!shouldBeNull.has_value());
    tests.push_back(!shouldBeNull2.has_value());
    
    for(int i = 0; i < tests.size(); i++){
        if(tests.at(i)){
            std::cout << "test " << i + 1 << " passed" << std::endl;
        } else {
            std::cout << "test " << i +1 << " failed" << std::endl;
            return;
        }
    }
}






