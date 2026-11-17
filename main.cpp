#include "server.h"
#include "models/Hasher.h"
#include <stdexcept>

int main(){
    try{
        std::cout << "running hasherTest" << std::endl;
        Hasher::test();
    } catch(std::runtime_error *e){
        std::cout << e->what() << std::endl;
    }
}
