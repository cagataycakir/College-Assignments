#ifndef ADMINIST_H
#define ADMINIST_H
#include <iostream>
#include <string>
#include "Security.h"

class Administrator{
    public:
        bool Login(std::string username, std::string password);
};

#endif