#include <iostream>
#include <string>

class Student{
    private:
        std::string name="UNDEFINED";
        int numClasses=0;
        std::string * classList;
    public:
        void changeName(std::string n){name=n;}
        void addCl(std::string c){
            if(numClasses==0){
                classList = new std::string [1];
                classList[0] = c;
            }else{
                std::string * temp = classList;
                classList = new std::string [numClasses+1];
                for(int a=0; a<numClasses; a++)
                    classList[a]=temp[a];
                delete[] temp;
                classList[numClasses]=c;
            }
            ++numClasses;
        }
        void rmCl(int a){
            if(numClasses==1 && a==0) {
                emptyCl();
                return;
            }
            if(numClasses<2 || a<0 || a>=numClasses) return;

            std::string * temp = classList;
            classList = new std::string [numClasses-1];
            for(int x=0; x<numClasses; x++){
                if(x<a) classList[x]=temp[x];
                if(x>a) classList[x-1]=temp[x];
            }
            delete[] temp;
            --numClasses;
        }
        void addNumCl(int a){
            if (a<1 || a>30) return;
            std::string buff;
            for(;a>0;a--){
                std::cin>>buff;
                addCl(buff);
                ++numClasses;
            }
        }
        std::string getClassN(int a){
            if(numClasses<1) return "Unattended";
            if(a<0 || a>=numClasses) return "Out of range";
            return classList[a];
        }
        int getClassC(){return numClasses;}
        void lsCl(){
            if (numClasses<1) return;
            std::cout<<"Student "<<name<<"'s classes are:\n";
            for(int a=0; a<numClasses;a++)
                std::cout<<a+1<<": "<<classList[a]<<"\n";
        }
        void emptyCl(){
            if(numClasses==0) return;
            delete[] classList;
            numClasses=0;
        }
        void operator=(Student out){
            emptyCl();
            for(int a=0; a<out.getClassC(); a++)
                addCl(out.getClassN(a));
        }
        ~Student(){
            if(numClasses>0) delete[] classList;
        }
};

int main(){
    Student a,b;
    a.changeName("Ali"); b.changeName("Veli");
    a.addCl("Physics");
    a.addCl("Math");
    a.addCl("Biology");

    a.lsCl();
    b=a;

    b.lsCl();
    b.emptyCl();

    b.addCl("Gym");

    b.lsCl();
    return 0;
}