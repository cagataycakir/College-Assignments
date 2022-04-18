#include "user.h"
#include "password.h"

using namespace Authenticate;

int main(){
    inputUserName();
    inputPassword();

    std::cout<<"Your username is "<<getUserName();
    std::cout<<" and your password is: "<<getPassword()<<"\n";

    return 0;
}