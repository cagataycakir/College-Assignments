#include "Security.h"
#include "Administrator.h"
#include "User.h"

void valCred(std::string login, std::string pass){
    User temp; Administrator tempA;
    if(temp.Login(login,pass)){
        std::cout<<"Welcome back "<<login;
        if(tempA.Login(login,pass)){
            std::cout<<", you have admin privileges.\n";
            return;
        }
        std::cout<<"\n";
        return;
    }
    std::cout<<"Login failed!\n";
    
}

int main(){
    valCred("Teacher1", "reallybadpass");
    valCred("costello", "tuesday");
    valCred("Teacher1", "wrongpass");

    return 0;
}