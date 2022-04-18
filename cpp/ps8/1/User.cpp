#include "User.h"

bool User::Login(std::string username, std::string password){
    Security val;
    if(val.validate(username,password)==0) return false;
    return true;
}