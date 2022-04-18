#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include "Security.h"

class User{
    public:
        bool Login(std::string username, std::string password);
};

#endif