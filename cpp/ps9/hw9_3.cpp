#include <iostream>

void flipBounds(int L, int R, char * ar, int count=0){
    if (L>=R) return;
    {
    char temp=ar[L];
    ar[L] = ar[R];
    ar[R]=temp;
    }
    count++;
    flipBounds(L+count,R-count,ar);
}

int main(){
    char text[]="This is cool.";
    flipBounds(8,11,text);
    std::cout<<text<<"\n";
    return 0;
}