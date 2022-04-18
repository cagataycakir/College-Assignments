#include <iostream>
#define MAX_SIZE 50

using namespace std;

int CheckSumPossibility(int num,int arr[],int size);

int main(){
    int arraySize;
    int arr[MAX_SIZE];
    int num;
    int returnVal;
    cin>>arraySize;
    cin>>num;

    for(int i=0;i<arraySize;++i){cin>>arr[i];}

    returnVal=CheckSumPossibility(num,arr,arraySize);

    if(returnVal==1){cout<<"Possible!"<<endl;}
    else {cout<<"Not possible!"<<endl;}

    return 0;
}

/*If the first item is smaller then the searched item 
search for the substraction for the following items in array.
If not returned, keep searching the same number on sub array items.
Recursive structure is indeed unefficient, but it works*/

int CheckSumPossibility(int num,int arr[],int size){
    if (size == 0) return 0;
    if (arr[0] == num) return 1;
    if (size == 1) return 0;
    if(num > arr[0])
        for (int i = 1; i < size; i++)
            if(CheckSumPossibility(num-arr[0],&arr[i],size-i) == 1) return 1;

    return CheckSumPossibility(num,&arr[1],size-1);
}