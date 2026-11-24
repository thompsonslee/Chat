#include "server.h"
#include "models/Hasher.h"
#include "Database.h"
#include "./repositories/UserRepo.h"
#include <stdexcept>

void runTests();

int main(){

    runTests();
    //Server server = Server();
    //server.start();
}

void runTests(){
    try{

        Hasher::test();

        Database db{};

        UserRepo userRepo(&db);
        userRepo.testUserRepo();
    } catch (std::runtime_error *e){
        std::string err (e->what());
        std::cout << err << std::endl;
    }
}
