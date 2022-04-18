#include <iostream>
#include <string>

class Polynomial{
    private:
        int * arr;
        int lenght=0;
    public:
        void addCoff(int a, int cof){
            if(cof<0 || cof>30) return;
            if(lenght==0)
                arr = new int[2];
            else{
                int * temp = arr;
                arr = new int[2*(lenght+1)];
                for(int a=0; a<lenght*2; a++)
                    arr[a] = temp[a];
                delete[] temp;
            }
            arr[lenght*2]=a; arr[lenght*2+1]=cof;
            ++lenght;
        }
        void removeArr(int r){
            if(r<0 || r>lenght) return;
            if(r==0 && lenght==1){
                delete[] arr;
                lenght=0;
            }
            int * temp = arr;
            arr = new int[2*(lenght-1)];
            for(int a=0; a<lenght*2; a++){
                if(a/2<r) arr[a] = temp[a];
                else if(a/2>r) arr[a-2] = temp[a];
            }
            delete[] temp;
            --lenght;
        }
        void emptyArr(){if(lenght==0)return; delete[] arr; lenght=0;}
        void setPol(int c, int m){
            if(m<0 || m>lenght) return;
            arr[m*2]=c;
        }
        int getPolCo(int ar){
            if(ar<0 || ar>lenght) return 0;
            return arr[ar*2];
        }
        int getPolMu(int ar){
            if(ar<0 || ar>lenght) return 0;
            return arr[ar*2+1];
        }
        int getPolLeng(){return lenght;}
        int getLeng(){return lenght;}
        void operator=(Polynomial right){
            emptyArr();
            for(int a=0; a<right.getLeng(); a++)
                addCoff(right.getPolCo(a),right.getPolMu(a));
        }
        int searchPolMu(int s){
            int a=0;
            std::cout<<s<<" <--\n";
            while(a<lenght && arr[a*2+1]!=s){std::cout<<arr[a*2+1]<<" <-\n";a++;}
            return a;
        }
        Polynomial operator+(Polynomial right){
            Polynomial ret;
            for(int a=0; a<lenght; a++)
                ret.addCoff(arr[a*2],arr[a*2+1]);
            for(int a=0, t; a<right.getLeng(); a++){
                std::cout<<right.getPolMu(a)<<"asdasdadfdf\n";
                t=ret.searchPolMu(right.getPolMu(a));
                std::cout<<t<<"\n";
                if(t==ret.getPolLeng()) ret.addCoff(right.getPolCo(t),right.getPolMu(t));
                else ret.setPol(right.getPolCo(a)+ret.getPolCo(t),t);
            }
            return ret;
        }
        Polynomial operator-(Polynomial right){
            Polynomial ret;
            for(int a=0; a<lenght; a++)
                ret.addCoff(-arr[a*2],arr[a*2+1]);
            for(int a=0, t; a<lenght; a++){
                t=ret.searchPolMu(right.getPolMu(a));
                if(t==-1)ret.addCoff(-right.getPolCo(a),right.getPolMu(a));
                else ret.setPol(ret.getPolCo(a)-right.getPolCo(t),t);
            }
            return ret;
        }
        Polynomial operator*(int right){
            Polynomial ret;
            printPol(); std::cout<<"<~in\n";
            for(int a=0; a<right; a++)
                ret=ret+*this;
            return ret;
        }
        Polynomial operator=(int right){
            Polynomial ret;
            for(int a=0; a<lenght; a++)
                ret.addCoff(arr[a*2],arr[a*2+1]);
            return ret;
        }
        void printPol(){
            for(int a=0; a<lenght; a++)
                std::cout<<arr[2*a]<<"x^"<<arr[2*a+1]<<" ";
            std::cout<<"\n";
        }
        ~Polynomial(){
            if(lenght>0) delete[] arr;
        }
};

Polynomial operator*(int left, Polynomial right){
    return right*left;
}

int main(){
    Polynomial a,b;
    a.addCoff(20,3);
    a.addCoff(7,2);
    a.addCoff(300,6);
    a.addCoff(2,1);
    b=a;
    b.printPol();
    b=a+b;
    b.printPol();
    //b=a*1;
    //b.printPol();
    //std::cout<<"outcome is:";
    //a.printPol();
    //2*a;
    //std::cout<<"aaaaa\n";
    //b=a;
    //std::cout<<"aaaaaa\n";
    //b.printPol();
    return 0;
}