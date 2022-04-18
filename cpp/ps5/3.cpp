#include <iostream>

bool primeChecker(int val);
class PrimeNumber{
    private:
        int val;
    public:
        PrimeNumber(){val=1;}
        PrimeNumber(int i){val=i;}
        int getNum(){return val;}
        void operator --(){
            if(val>3){
                --val;
                while(!primeChecker(val))
                    --val;
            }
        }
        void operator --(int){
            if(val>3){
                --val;
                while(!primeChecker(val))
                    --val;
            }
        }
        void operator ++(){
            ++val;
            while(!primeChecker(val))
                ++val;
        }
        void operator ++(int){
            ++val;
            while(!primeChecker(val))
                ++val;
        }
};

int main(){
    PrimeNumber a(13);
    std::cout<<a.getNum()<<"\n";
    ++a;
    std::cout<<a.getNum()<<"\n";
    --a;
    std::cout<<a.getNum()<<"\n";
    a++;
    std::cout<<a.getNum()<<"\n";
    a--;
    std::cout<<a.getNum()<<"\n";
    return 0;
}

bool primeChecker(int val){
    for(int d=2; d<val/2; ++d)
        if(val%d==0)return false;
    return true;
}