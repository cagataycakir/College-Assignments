#include <iostream>

class Vector2D{
    private:
        int x;
        int y;
    public:
        Vector2D(){x=0;y=0;}
        Vector2D(int a, int b){x=a;y=b;}
        int valX(){return x;}
        int valY(){return y;}
        void setX(int x){this->x=x;}
        void setY(int y){this->y=y;}
};

int operator * (Vector2D &ob1, Vector2D &ob2){
    return ob1.valX() * ob2.valX() + ob1.valY() * ob2.valY();
}

int main(){
    Vector2D a(1,2), b(3,4);
    std::cout<<a*b<<"\n";
    return 0;
}