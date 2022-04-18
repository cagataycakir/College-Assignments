#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>

using namespace std;

string getRndmArr(int num); //gives a scrambled number according to size
int * checkGuess(string realN, string guessN); //checks accuracy
void masErr(int errCode); //printsout related error codes
char sanitM(string arg); //sanitizes the mode input
int sanitR(string arg); //sanitizes the -r input
string stringCheck(string in); //converts arrays and strings to to usable format if valid
void game(string & realNum); //main game loop when initialising is valid

int main(int argc, char * argv[]){
    if(argc != 3) masErr(0);
    else if (sanitM(argv[1]) == 'r'){ //r mode
        int numC=sanitR(argv[2]);
        if(numC){
            //generating secret num
            string realNum=getRndmArr(numC);
            game(realNum);            
        }else masErr(0);
    }else if (sanitM(argv[1]) == 'u'){ //u mode
        //getting secret num
        string realNum = stringCheck(argv[2]);

        if(realNum.size()) game(realNum); 
        else masErr(0);
    }else masErr(0);

    return 0;
}

string getRndmArr(int num){
    srand(time(NULL));
    string ret;

    if(num<11) {
        string nums ="0123456789"; //create number list
        string numsB; //number list buffer

        for(int a=0; a<num; ++a){
            int mypick = rand() % (10-a); //pick a random number from num list
            if(!a && !mypick) ++mypick; //if 0 is picked for first row, skip to next
            ret.push_back(nums[mypick]); //assign number

            //extract used number from list by resize and pushbacks, using buffer vector stack
            numsB = nums; nums.clear();
            for(int b=0; b<numsB.size(); ++b)
                if(b!=mypick)nums.push_back(numsB[b]);
        }
    }else ret.clear(); //error handling(unlikely)

    return ret;
}

int * checkGuess(string realN, string guessN){
    int * ret = new int[2];
    bool numBuf[10]; //for checking already counted numbers
    for(int a=0; a<10; ++a) numBuf[a]=true;

    if(realN.size() != guessN.size()) ret[0]=-1; //input error handling(unlikely)
    else{
        ret[0]=ret[1]=0;
        //look for each element and try to find a match
        //if matching element is at same row increment the first
        //if in a different row increment second
        for(int a=0; a<realN.size(); ++a){
            while(!numBuf[(int)guessN[a]-'0']) ++a; //skip if already counted
            for(int b=0; b<realN.size(); ++b){
                if(guessN[a]==realN[b]){
                    if(a==b) ++ret[0];
                    else ++ret[1];
                    b=realN.size(); //end check cycle
                    numBuf[(int)(guessN[a]-'0')] = false; //flag counted
                }
            }
        }
    }
    return ret;
}

string stringCheck(string in){
    string ret;
    for(int a=0; a<in.size();++a){
        if(in[a]>='0' && in[a]<='9') ret.push_back(in[a]);
        else{ret.clear(); return ret;} //invalid chars cause loop broke and string gets cleared
    }
    return ret;
}

void game(string & realNum){
    //string and vector buffer for loop and condition checks
    string inNum;

    for(int a=0; a<100; ++a){
        inNum.clear();//clear string buffer
                
        cin>>inNum; //put user input to string buffer then send it to correction
        inNum=stringCheck(inNum);
                
        if(1>inNum.size()) {masErr(2); return;} //error case 2(assumption of stringCheck emptying invalid inputs)
        else if(realNum.size()!=inNum.size()){masErr(1); return;} //error case 1(assumption of unmatching buffer sizes)
        else{      
            //looking at guess
            int * result = checkGuess(realNum, inNum);

            if(result[0]==realNum.size()) {cout<<"FOUND "<<a+1<<endl; return;}
            else if(result[0]!=-1) cout<< result[0]<<" "<< result[1]<<endl;
            else {masErr(0); return;}
            delete result;
        }
    }
    cout<<"FAILED\n";
    return;
}

void masErr(int errCode){
    switch(errCode){
        case 1:
            cout<<"E1\n";
            break;
        case 2:
            cout<<"E2\n";
            break;
        default:
            cout<<"E0\n";
    }
}

int sanitR(string arg){
    int num = atoi(arg.c_str());
    if ( num>0 && num<11) return num;
    else return 0;
}

char sanitM(string arg){

    if(arg.size() == 2 && arg[0]=='-')
        switch(arg[1]){
            case 'r':
                return 'r';
            case 'u':
                return 'u';
            default:
                return '0';
        }
    else return '0';
}