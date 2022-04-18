#include <iostream>
#include <math.h>

class MyInteger{
    private:
        int val;
    public:
        MyInteger(){val=0;}
        MyInteger(int i){val=i;}
        int getVal(){return val;}
        void setVal(int a){val=a;}
        int operator [] (int i) {
            i=pow(10,i);
            if(i>val) return -1;
            else {i=val/i; i=i%10; return i;}
        }
};

int main(){
    MyInteger a(200);
    std::cout<<a[2]<<"\n";
    std::cout<<a[5]<<"\n";
    return 0;
}