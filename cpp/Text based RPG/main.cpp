#include "creature.h"

int main(){
    srand(time(NULL));
    Human testH(0,1,1);
    Balrog testB(2,3,3);
    std::cout<<"Works!\n";
    testH.getDamage();
    testB.getDamage();
    return 0;
}