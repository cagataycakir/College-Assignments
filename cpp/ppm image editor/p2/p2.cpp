#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>

class PPM{
    private:
        std::string fName;
        int dimCol=0, dimRow=0, maxC=0, dynS=0;
        int * myArr;
        void purgeArr(){if(dynS==0)return; delete[] myArr; dimCol=dimRow=maxC=dynS=0;}
        void appendArr(int in);
    public:
        PPM(){}
        PPM(std::string file){fName=file; upClassPPM();}
        ~PPM(){purgeArr();}
        void upClassPPM();
        void setFName(std::string newF){fName=newF;}
        std::string getFName(){return fName;}
        void printVec(int vecN){std::cout<<myArr[vecN]<<"\n";}
        void printMx(){std::cout<<maxC<<"\n";}
        void graySC(double c_r, double c_g, double c_b);
        void save();
        void setArr(int arr, int val){if(arr>=0 && arr<dynS) myArr[arr]=val;}
        PPM operator+(PPM right);
        void operator=(PPM right);
        int getCol(){return dimCol;}
        int getRow(){return dimRow;}
        int getMaxC(){return maxC;}
        int getArr(int in){if(in>=0 && in<dynS)return myArr[in]; return -1;}
};

void menuM(); //main menu
std::string menuOp(); //file opening menu
std::string menuSa(); //save menu
void menuScr(PPM im[2], int &sel); //script menu
int sanitizedIn();//auto sanitized input collector
bool sanitizedCo(double &r, double &g, double &b);//auto sanitized coefficient input collector
bool nameVal(std::string in);//file name validation
bool fileVal(std::string in);//file validation
void scrGray(PPM &im);//grayscaling script
void scrSOTWarn(PPM im[2]);//sum of two script

void invOp(); //invalid option descriptor

int main(){

    menuM();

    return 0;
}

void PPM::appendArr(int in){
    if(dynS!=0){
        int * temp = myArr;
        myArr = new int[dynS+1];
        for(int a=0; a<dynS; a++) myArr[a]=temp[a];
        delete[] temp;
    }else
        myArr = new int[1];

    myArr[dynS]=in;
    dynS++;
}

void PPM::upClassPPM(){
    if(fName.substr(fName.find_last_of('.'))!=".ppm")exit(1); //if invalid file extention

    std::ifstream fp(fName);
    if(!fp.is_open()) exit(1);//if failed to open file

    std::string buff;
    getline(fp,buff);
    if(buff!="P3"){fp.close();exit(1);}//if invalid image format

    purgeArr();
    //get dimensions
    do{getline(fp,buff);}while(buff[0]=='#');//skip comment lines
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
        int numBuff=-1;
        for(int a=0;a<buff.size();++a){
            if(buff[a]>='0' && buff[a]<='9'){
                if(numBuff==-1) numBuff=0;
                numBuff*=10;
                numBuff+=buff[a]-'0';
            }else if(buff[a]==' '){
                if(a!=0 || buff[a-1]!=' '){
                    appendArr(numBuff);
                    numBuff=-1;
                }
            }
            else{fp.close(); exit(1);} //format error
        }
        if(numBuff!=-1)appendArr(numBuff);
    }

    if(dimCol*dimRow*3!=dynS){fp.close(); exit(1);}//format error

    fp.close();
}

void PPM::save(){
    if(fName.substr(fName.find_last_of('.'))!=".ppm")exit(1); //if invalid file extention

    std::ofstream fp(fName, std::ofstream::trunc);
    if(!fp.is_open()) exit(1);//if failed to open file

    fp<<"P3\n";
    fp<<dimCol<<" "<<dimRow<<"\n"<<maxC<<"\n";

    for(int a=0; a<dynS; ++a){
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

PPM PPM::operator+(PPM r){
    PPM ret;
    if(maxC==r.getMaxC() && dimCol==r.getCol() && dimRow==r.getRow()){
        ret=r;
        int size=dimCol*dimRow*3; int temp;
        for(int a=0; a<size; a++){
            temp=r.getArr(a)+myArr[a]; if(temp>maxC) temp=maxC;
            ret.setArr(a,temp);
        }
    }
    return ret;
}

void PPM::operator=(PPM r){
    purgeArr();
    if(r.getMaxC()!=maxC) maxC=r.getMaxC();
    dimRow=r.getRow(); dimCol=r.getCol();
    int size=dimRow*dimCol*3;
    if (size==0) return;
    for(int a=0; a<size; a++)
        appendArr(r.getArr(a));
}

void menuM(){
    int in, memP=0; std::string myFile; PPM myIm[2];
    do{
        std::cout<<"MAIN MENU\n";
        std::cout<<"(Memory "<<memP+1<<" is selected.)\n\n";
        std::cout<<"0 - Exit\n1 - Open An Image\n";
        std::cout<<"2 - Save Image Data\n3 - Scripts\n";
        std::cout<<"4 - Change memory\n\n";
        in=sanitizedIn();
        switch (in){
            case 0: break;
            case 1:
                myFile = menuOp();
                if(myFile.size()>0){
                    myIm[memP].setFName(myFile);
                    myIm[memP].upClassPPM();
                }
                break;
            case 2:
                myFile = menuSa();
                if(myFile.size()>0){
                    myIm[memP].setFName(myFile);
                    myIm[memP].save();
                }
                break;
            case 3:
                menuScr(myIm, memP);
                break;
            case 4:
                memP=(memP+1)%2;
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
        std::cout<<"0 - Back\n1 - Enter A File Name\n\n";
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

void menuScr(PPM im[2], int &sel){
    int in; double c_r, c_g, c_b; std::string coRet;
    do{
        std::cout<<"(Memory "<<sel+1<<" is selected.)\n\n";
        std::cout<<"0 - Back\n1 - Grayscale\n2 - Sum Of Two\n3 - Change memory\n\n";
        in=sanitizedIn();
        switch (in){
            case 0: break;
            case 1:
                scrGray(im[sel]);
                break;
            case 2:
                scrSOTWarn(im);
                break;
            case 3:
                sel=(sel+1)%2;
                break;
            default:
                invOp();
        }
    }while(in!=0);
}

void scrGray(PPM &im){
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

void scrSOTWarn(PPM im[2]){
    PPM temp=im[0]+im[1]; int in; std::string fName;
    std::cout<<"Sum of two images in memory has been generated. ";
    std::cout<<"What do you want to do with the data?\n";
    std::cout<<"0 - Save to disk.\n1 - Overwrite memory 1.\n2 - Overwrite memory 2. \n3 - Discard\n\n";

    in=sanitizedIn();
    switch (in){
        case 0:
            fName=menuSa();
            if(fName.size()>0){
                temp.setFName(fName);
                temp.save();
            }else
                std::cout<<"Data is not saved.\n";
            break;
        case 1:
            im[0]=temp;
            break;
        case 2:
            im[1]=temp;
            break;
        default:
            invOp();
        case 3:
            std::cout<<"Data is not saved.\n";
    }

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