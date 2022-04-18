#include <iostream>
#include <string>

void reverseWord(std::string & word){
    if (word.size()<2) return;
    char temp;

    for(int pH=0, pT=word.size()-1; pH<pT; pH++ ,pT--){
        temp=word[pH]; word[pH]=word[pT]; word[pT]=temp;
    }
}

int main(){
    std::string myWord;
    std::cin>>myWord;

    reverseWord(myWord);
    
    std::cout<<"\nReverse is:"<<myWord<<"\n";

    return 0;
}