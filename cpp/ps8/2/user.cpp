#include "user.h"

std::string username="N/A";

namespace{
    bool isValid(){
        std::string abc;
        for(int i=0; i<26; i++){
            abc+=(char)(i+'a');
            abc+=(char)(i+'A');
        }
        if(username.size()==8 && username.find_first_not_of(abc)==std::string::npos) return true;
        return false;
    }
}

void Authenticate::inputUserName( ){
    do{
        std::cout << "Enter your username (8 letters only)\n";
        std::cin >> username;
    } while (!isValid());
}

std::string Authenticate::getUserName( ){
    return username;
}