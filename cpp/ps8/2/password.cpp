#include "password.h"

std::string password="N/A";

namespace{
    bool isValid(){
        std::string abc;
        for(int i=0; i<26; i++){
            abc+=(char)(i+'a');
            abc+=(char)(i+'A');
        }
        std::string nabc="0123456789-+._"; //acceptable non-alphabethic

        //incorrect size
        if(password.size()!=8) return false;
        //invalid characters
        if(password.find_first_not_of(abc+nabc) != std::string::npos) return false;
        //if there are both alphabetic and non alphabetic characters good to go
        if(password.find_first_of(nabc) != std::string::npos &&
        password.find_first_of(abc) != std::string::npos) return true;

        return false;
    }
}

void Authenticate::inputPassword(){
    do{
        std::cout << "Enter your password (at least 8 characters and at least one non-letter)\n";
        std::cin >> password;
    }while (!isValid( ));
}
std::string Authenticate::getPassword(){
    return password;
}