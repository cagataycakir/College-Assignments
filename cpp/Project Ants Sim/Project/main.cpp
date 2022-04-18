#include "header.h"

using namespace std;

int main(){
    srand(time(NULL));
    World test;

    for (int i = 0; i < 100; i++) test.addAnt(false);
    for (int i = 0; i < 3; i++) test.addDoodle();
    system("clear");
    test.printGraph();
    cout<<test.bugC()<<endl;
    for (int i = 0; i < 200; i++){
        sleep(1);
        system("clear");
        test.moveAll();
        test.printGraph();
        cout<<test.bugC()<<endl;
    }
    return 0;
}