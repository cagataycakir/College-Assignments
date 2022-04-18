#include <iostream>
#include <string>
#include <vector>

class Student{
    private:
        std::string name="UNDEFINED";
        int numClasses=0;
        std::vector<std::string>classList;
    public:
        void changeName(std::string n){name=n;}
        void addCl(std::string c){classList.push_back(c);++numClasses;}
        void rmCl(int a){
            if(a>=numClasses) return;
            classList.erase(classList.begin()+a);--numClasses;
        }
        void addNumCl(int a){
            if (a<1 || a>30) return;
            std::string buff;
            for(;a>0;a--){
                std::cin>>buff;
                classList.push_back(buff);
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
            classList.clear();
            numClasses=0;
        }
        void operator=(Student out){
            emptyCl();
            for(int a=0; a<out.getClassC(); a++)
                addCl(out.getClassN(a));
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