#include <iostream>

using namespace std;

struct stack{
    int num;
    stack * next=nullptr;
    stack(){num = 0;}
    stack(int i){num = i;}
    void addStack(int i){
        if(next == nullptr) next = new stack(i);
        else next->addStack(i);
    }
    void destroyStack(){
        if(next != nullptr){
            next->destroyStack();
            delete next;
            next=nullptr;
        }
    }
    void printStack(){
        cout<<num<<" ";
        if(next != nullptr) next->printStack();
    }
    int getStackSize(){
        if(next==nullptr) return 1;
        return 1+next->getStackSize();
    }

    int getStack(int a){
        if(a==1 || a==0) return num;
        stack * ptr = next;
        while(a>2 && ptr!=nullptr){
            ptr=ptr->next;
            a--;
        }
        return ptr->num;
    }
    void setStack(int a, int nu){
        if(a==1 || a==0){num=nu; return;}
        stack * ptr = next;
        while(a>2 && ptr!=nullptr){
            ptr=ptr->next;
            a--;
        }
        ptr->num = nu;
    }
    ~stack(){destroyStack();}
};

void foo(stack &myS){
    int size = myS.getStackSize();
    for (int i = 1; i < size/2+1; i++){
        int temp=myS.getStack(i);
        myS.setStack(i,myS.getStack(size-i+1));
        myS.setStack(size-i+1,temp);
    }
    
}

int main(){
    stack test(1);
    test.addStack(2);
    test.addStack(3);
    test.addStack(4);
    test.addStack(5);
    test.addStack(6);
    test.addStack(7);
    test.printStack(); cout<<"\n";
    foo(test);
    test.printStack(); cout<<"\n";

    return 0;
}