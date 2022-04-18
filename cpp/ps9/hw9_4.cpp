#include <iostream>

void towerH(int block, int startCol=1, int buffCol=2, int endCol=3){
    if(block==1){
        std::cout<< startCol << " -> " << endCol << "\n";
        return;
    } else if(block>1) {
        towerH(block-1, startCol, endCol, buffCol);
        std::cout<< startCol << " -> " << endCol << "\n";
        towerH(block-1, buffCol, startCol, endCol);
    } else {
        //handle error
        exit(1);
    }
}

int main(){
    towerH(3);
    return 0;
}