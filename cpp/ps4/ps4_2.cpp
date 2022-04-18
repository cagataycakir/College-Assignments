#include <iostream>
#include <vector>
#define DEF_V 6

int main(int argc, char ** argv){
    int vecsize=DEF_V;
    //if there is single argument and atoi assigns vecsize value to 0
    //reassign vecsize
    if(argc == 2 && !( vecsize = atoi(argv[1]))) vecsize=DEF_V;
    std::vector<int>suitors; int roll=0, counter=0;

    for(int a=0; a<vecsize; a++) suitors.push_back(a+1);

    while(suitors.size()>1){
        if(counter==2){
            for(int a=0; a<suitors.size(); a++) std::cout<<suitors[a];
            std::cout<<"\n"<<suitors[roll]<<" is eliminated.\n";
            suitors.erase(suitors.begin()+roll);
        }else ++roll%=suitors.size();//incrementing then mod
        ++counter; counter%=3;
    }

    std::cout<<"Winner is:"<<suitors[0]<<"\n";

    return 0;
}