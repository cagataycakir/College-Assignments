#include <iostream>

class Point{
    int x=0;
    int y=0;
    public:
    void setCoor(int x, int y){this->x=x;this->y=y;}
    void setRelCoor(int x, int y){this->x+=x;this->y+=y;}
    void rotCl90(){int temp=y; y=-x; x=temp;}
    int getX(){return x;}
    int getY(){return y;}
};

int main(){
    Point myPoint;
    myPoint.setCoor(3,4);
    std::cout<<"Mypoint current x is: "<<myPoint.getX();
    std::cout<<" current y is: "<<myPoint.getY()<<"\n";
    myPoint.setRelCoor(1,-1);
    std::cout<<"Mypoint current x is: "<<myPoint.getX();
    std::cout<<" current y is: "<<myPoint.getY()<<"\n";
    myPoint.rotCl90();
    std::cout<<"Mypoint current x is: "<<myPoint.getX();
    std::cout<<" current y is: "<<myPoint.getY()<<"\n";
    return 0;
}