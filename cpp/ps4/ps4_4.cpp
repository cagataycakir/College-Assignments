#include <iostream>
#include <vector>

class Pizza{
    char type='0'; //d h or p
    char size='0'; //s m or l
    int pepC=0;
    int cheeC=0;

    public:
    void editPizza(char t,char s,int p, int c){
        type=t; 
        size=s;
        pepC=p;
        cheeC=c;
    }
    char getType(){return type;}
    char getSize(){return size;}
    int getPep(){return pepC;}
    int getChee(){return cheeC;}

    void setType(char &t){type=t;}
    void setSize(char &s){size=s;}
    void setPep(int &p){pepC=p;}
    void setChee(int &c){cheeC=c;}

    void outputDescription();
    double computePrice();
};

int main(){
    Pizza myPiz1;
    myPiz1.editPizza('d','s',1,1);

    std::cout<<"I got: ";
    myPiz1.outputDescription();
    std::cout<<".\nI paid $"<<myPiz1.computePrice()<<" bucks for it. It wasn't even that good.\n";
    return 0;
}

double Pizza::computePrice(){
    double ret=0;
    switch(size){
        case 's': case 'S':
            ret=10;
            break;
        case 'm': case 'M':
            ret=14;
            break;
        case 'l': case 'L':
            ret=17;
            break;
        default:
            return -1;
    }
    ret+=(pepC+cheeC)*2;
    return ret;
}

void Pizza::outputDescription(){
    if(computePrice()==-1) return;

    switch(size){
        case 's': case 'S':
            std::cout<<"Small";
            break;
        case 'm': case 'M':
            std::cout<<"Medium";
            break;
        case 'l': case 'L':
            std::cout<<"Large";
    }
    std::cout<<" ";

    switch(type){
        case 'd': case 'D':
            std::cout<<"deep dished ";
            break;
        case 'h': case 'H':
            std::cout<<"hand tossed ";
            break;
        case 'p': case 'P':
            std::cout<<"panned ";
    }
    std::cout<<"pizza";
}