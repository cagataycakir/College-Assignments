#include <iostream>
#define ST_C 5

char grad(double point);

struct student{
    void updateRec(double q1, double q2, double e1, double e2);
    double getAvarage(){return avrg;}
    char getGrade(){return grade;}

    private:
    double quiz1=0;
    double quiz2=0;
    double exam1=0;
    double exam2=0;
    double avrg=0;
    char grade='N';
};

int main(){
    double myData[ST_C][4]=
    {
        {7,10,90,95},
        {9,8,90,80},
        {7,8,70,80},
        {5,8,50,70},
        {4,0,40,35}
    };
    student myClass[ST_C];

    for(int a=0; a<ST_C; a++){
        myClass[a].updateRec(myData[a][0],myData[a][1],myData[a][2],myData[a][3]);
        std::cout<<"Student no:"<<a+1<<". Avarage is: "<<myClass[a].getAvarage()<<" and grade is: "<< myClass[a].getGrade()<<"\n";
    }

    return 0;
}

char grad(double point){
    if(point>=0 && point<=100){
        if(point>=90) return 'A';
        else if(point>=80) return 'B';
        else if(point>=70) return 'C';
        else if(point>=60) return 'D';
        return 'F';
    }
    return 'N';
}

void student::updateRec(double q1, double q2, double e1, double e2){
    quiz1=q1; quiz2=q2; exam1=e1; exam2=e2;
    avrg=((quiz1+quiz2)*5+exam1+exam2*2)/4;
    grade=grad(avrg);
}