#include "Administrator.h"

bool Administrator::Login(std::string username, std::string password){
    Security val;
    if(val.validate(username,password)==0) return false;
    if(username=="Teacher1") return true;
    if(username=="Teacher2") return true;
    return false;
}