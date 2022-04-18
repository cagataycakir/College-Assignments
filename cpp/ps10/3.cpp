#include <iostream>
#include <string>

using namespace std;

class Document{
    public:
        string getText(){return text;}
        void setText(string in){text=in;}
        void operator=(string right){text=right;}
    protected:
        string text;
};

class Email : public Document{
    public:
        using Document::operator=;
        string getSender(){return sender;}
        string getRecip(){return recipient;}
        string getTitle(){return title;}
        void setSender(string s){sender=s;}
        void setRecip(string r){recipient=r;}
        void setTitle(string t){title=t;}
        void operator=(Email& r){
            text=r.text; sender=r.sender;
            recipient=r.recipient; title=r.title;
        }
    protected:
        string sender;
        string recipient;
        string title;
};

class File : public Document{
    public:
        using Document::operator=;
        string getPath(){return pathName;}
        void setPath(string p){pathName=p;}
        void operator=(File& r){text=r.text; pathName=r.pathName;}
    protected:
        string pathName;
};

bool ContainsKeyword(Document& docObject, string keyword){
    if(docObject.getText().find(keyword) != string::npos)
    return true;
    return false;
}

int main(){
    Document test, test1; Email test2, test3; File test4, test5;

    test="Some test text.\n"; test1="Another test text in c++.\n";
    test2="Some test email in c++.\n"; test3="Another test text.\n";
    test4="Some test file in c++.\n"; test5="Another test file.\n";

    if(ContainsKeyword(test,"c++")) cout<<"0 contains keyword\n";
    if(ContainsKeyword(test1,"c++")) cout<<"1 contains keyword\n";
    if(ContainsKeyword(test2,"c++")) cout<<"2 contains keyword\n";
    if(ContainsKeyword(test3,"c++")) cout<<"3 contains keyword\n";
    if(ContainsKeyword(test4,"c++")) cout<<"4 contains keyword\n";
    if(ContainsKeyword(test5,"c++")) cout<<"5 contains keyword\n";

    return 0;
}