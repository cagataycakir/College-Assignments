#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>

class PPM{
    private:
        std::string fName;
        int dimCol=0, dimRow=0, maxC=0;
        std::vector<int>myArr;
    public:
        PPM(){}
        PPM(std::string file){fName=file; upClassPPM();}
        void upClassPPM();
        void setFName(std::string newF){fName=newF;}
        std::string getFName(){return fName;}
        void printVec(int vecN){std::cout<<myArr[vecN]<<"\n";}
        void printMx(){std::cout<<maxC<<"\n";}
        void graySC(double c_r, double c_g, double c_b);
        void save();
};

void menuM(); //main menu
std::string menuOp(); //file opening menu
std::string menuSa(); //save menu
void menuScr(PPM &im); //script menu
int sanitizedIn();//auto sanitized input collector
bool sanitizedCo(double &r, double &g, double &b);//auto sanitized coefficient input collector
bool nameVal(std::string in);//file name validation
bool fileVal(std::string in);//file validation

void invOp(); //invalid option descriptor

int main(){

    menuM();

    return 0;
}

void PPM::upClassPPM(){
    if(fName.substr(fName.find_last_of('.'))!=".ppm")exit(1); //if invalid file extention

    std::ifstream fp(fName);
    if(!fp.is_open()) exit(1);//if failed to open file

    std::string buff;
    getline(fp,buff);
    if(buff!="P3"){fp.close();exit(1);}//if invalid image format

    //get dimensions
    do{getline(fp,buff);}while(buff[0]=='#');//skip comment lines
    myArr.clear();
    for(int a=0, b=0; a<buff.size();++a){
        if(b>1) {fp.close(); exit(1);} //format error
        if(buff[a]>='0' && buff[a]<='9'){
            if(b==0) {dimCol*=10; dimCol+=(buff[a]-'0');}
            else {dimRow*=10; dimRow+=(buff[a]-'0');}
        }
        else if(buff[a]==' ' && a>0 && buff[a-1]!=' ')++b;
    }

    if(dimCol==0 || dimRow==0){fp.close(); exit(1);}//format error

    //get max val
    getline(fp,buff);
    for(int a=0, b=0; a<buff.size();++a){
        if(b>0) {fp.close(); exit(1);} //format error
        if(b==0 && buff[a]>='0' && buff[a]<='9'){
            maxC*=10;
            maxC+=(buff[a]-'0');
        }
        else if(buff[a]==' ' && a>0 && buff[a-1]!=' ')++b;
    }

    //get pixels
    while(getline(fp,buff)){
        for(int a=0;a<buff.size();++a){
            if(buff[a]>='0' && buff[a]<='9'){
                if(a==0 || buff[a-1]==' ') myArr.push_back(buff[a]-'0');
                else if(buff[a-1]>='0' && buff[a-1]<='9'){
                    myArr[myArr.size()-1]*=10;
                    myArr[myArr.size()-1]+=buff[a]-'0';
                }
            }else if(buff[a]!=' ') {fp.close(); exit(1);} //format error
        }
    }

    if(myArr.size()!=dimCol*dimRow*3){fp.close(); exit(1);} //data error

    fp.close();
}

void PPM::save(){
    if(fName.substr(fName.find_last_of('.'))!=".ppm")exit(1); //if invalid file extention

    std::ofstream fp(fName, std::ofstream::trunc);
    if(!fp.is_open()) exit(1);//if failed to open file

    fp<<"P3\n";
    fp<<dimCol<<" "<<dimRow<<"\n"<<maxC<<"\n";

    for(int a=0; a<myArr.size(); ++a){
        fp<<myArr[a];
        if(a%(dimCol*3)==(dimCol*3)-1) fp<<"\n";
        else fp<<" ";
    }

    fp.close();
}

void PPM::graySC(double c_r, double c_g, double c_b){
    for(int a=0; a<dimCol*dimRow; ++a){
        myArr[a*3]=myArr[a*3]*c_r+myArr[a*3+1]*c_g+myArr[a*3+2]*c_b;
        if(myArr[a*3]>maxC) myArr[a*3]=maxC; //saturation handling
        myArr[a*3+1]=myArr[a*3];
        myArr[a*3+2]=myArr[a*3];
    }
}

void menuM(){
    int in; std::string myFile; PPM myIm;
    do{
        std::cout<<"MAIN MENU\n\n";
        std::cout<<"0 - Exit\n1 - Open An Image(D)\n";
        std::cout<<"2 - Save Image Data(D)\n3 - Scripts(D)\n\n";
        in=sanitizedIn();
        switch (in){
            case 0: break;
            case 1:
                myFile = menuOp();
                if(myFile.size()>0){
                    myIm.setFName(myFile);
                    myIm.upClassPPM();
                }
                break;
            case 2:
                myFile = menuSa();
                if(myFile.size()>0){
                    myIm.setFName(myFile);
                    myIm.save();
                }
                break;
            case 3:
                menuScr(myIm);
                break;
            default:
                invOp();
        }
    }while(in!=0);
}

std::string menuOp(){
    int in; std::string ret;
    do{
        std::cout<<"0 - UP\n1 - Enter The Name Of The Image File\n\n";
        in=sanitizedIn();
        switch (in){
            case 0: break;
            case 1:
                std::cin>>ret;
                if(!fileVal(ret)) ret.clear();
                std::cin.clear(); fflush(stdin);
                break;
            default:
                invOp();
        }
    }while(in!=0);

    return ret;
}

std::string menuSa(){
    int in; std::string ret;
    do{
        std::cout<<"0 - UP\n1 - Enter A File Name\n\n";
        in=sanitizedIn();
        switch (in){
            case 0: break;
            case 1:
                std::cin>>ret;
                std::cin.clear(); fflush(stdin);
                if(!nameVal(ret)) ret.clear();
                break;
            default:
                invOp();
        }
    }while(in!=0);

    return ret;
}

void menuScr(PPM &im){
    int in; double c_r, c_g, c_b; std::string coRet;
    do{
        std::cout<<"0 - UP\n1 - Enter Coefficients For RED GREEN And BLUE Channels.\n\n";
        in=sanitizedIn();
        switch (in){
            case 0: break;
            case 1:
                c_r=c_g=c_b=0;
                if(!sanitizedCo(c_r,c_g,c_b))
                    std::cout<<"UNCLEAR/INVALID COEFFICIENTS INPUT PLEASE TRY AGAIN.\n";
                else im.graySC(c_r,c_g,c_b);
                break;
            default:
                invOp();
        }
    }while(in!=0);
}

int sanitizedIn(){
    std::string buff;
    std::cin>>buff;
    std::cin.clear(); fflush(stdin);
    if(buff.find_first_not_of("0123456789")!=-1) return -1;
    else return atoi(buff.c_str());
}

bool sanitizedCo(double &r, double &g, double &b){
    std::string buff; double temp;

    for(int c=0; c<3; c++){
        std::cin>>buff;
        temp=atof(buff.c_str());

        if(!(temp<1 && temp>0)){
            std::cin.clear(); fflush(stdin);
            return false;
        }

        switch (c){
            case 0: r=temp; break;
            case 1: g=temp; break;
            case 2: b=temp; break;
        }
    }
    std::cin.clear(); fflush(stdin);
    return true;
}

bool nameVal(std::string in){
    if(in.size()<5 || in.find_last_of(".")==-1 || in.substr(in.find_last_of("."))!=".ppm" ){
        std::cout<<"BAD OR INVALID NAME PLEASE TRY AGAIN\n\n\n";
        return false;
    }else if(in.size()>20){
        std::cout<<"LONG FILENAME PLEASE TRY AGAIN(should be less then 21)\n\n\n";
        return false;
    }
    return true;
}

bool fileVal(std::string in){
    if(!nameVal(in)) return false;

    std::ifstream fp(in);
    if(!fp.good()){
        std::cout<<"FILE DOES NOT EXIST PLEASE TRY AGAIN\n\n\n";
        return false;
    }
    else{fp.close(); return true;}
}

void invOp(){
    std::cout<<"OPTION IS INVALID PLEASE TRY AGAIN\n\n\n";
}