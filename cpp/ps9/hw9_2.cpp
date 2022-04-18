#include <iostream>

void flipBounds(int L, int R, char * ar){
    if (L>=R) return;
    {
    char temp=ar[L];
    ar[L] = ar[R];
    ar[R]=temp;
    }
    flipBounds(L+1,R-1,ar);
}

int main(){
    char text[]="This is cool.";
    flipBounds(8,11,text);
    std::cout<<text<<"\n";
    return 0;
}