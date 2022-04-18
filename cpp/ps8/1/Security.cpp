#include "Security.h"

int Security::validate(std::string username, std::string password){
    if ((username=="abbott") && (password=="monday")) return 1;
    if ((username=="costello") && (password=="tuesday")) return 2;
    if ((username=="Teacher1") && (password=="reallybadpass")) return 3;
    if ((username=="Teacher2") && (password=="qwerty")) return 4;
    return 0;
}