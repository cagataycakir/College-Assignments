#ifndef SECURITY_H
#define SECURITY_H
#include <iostream>
#include <string>

class Security{
    public:
        static int validate(std::string username, std::string password);
};

#endif