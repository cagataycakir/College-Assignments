#include <iostream>
#include <string>

using namespace std;

class Payment{
    public:
        Payment(){}
        Payment(float a){amount = a;}
        void setAmo(float& in){amount = in;}
        float getAmo(){return amount;}
        virtual void paymentDetails(){cout<<"You have "<<amount<<" dollar in your account.\n";}
    protected:
        float amount;
};

class CashPayment : public Payment {
    public:
        CashPayment(float a){amount = a;}
        void paymentDetails() override {
            cout<<"You have "<<(int)amount<<" dollar in your account.\n";
        }
};

class CreditCardPayment : public Payment {
    public:
        CreditCardPayment(string ca, int e, int c, float a){
            cardName=ca;
            expiration=e;
            cardNumber=c;
            amount=a;
        }
        void paymentDetails() override {
            cout<<"Username:"<<cardName
            <<"\nExpiration year:"<<expiration<<"\nNo:"<<cardNumber
            <<"\nYou have "<<(int)amount<<" dollar in your account.\n";}
    protected:
        string cardName;
        int expiration;
        int cardNumber;
};

int main(){
    CashPayment test1(100.0F), test2(200.0F);
    CreditCardPayment test3("John",2021,33455,400.0F), test4("Alex",2023,22245,300.66F);

    test1.paymentDetails(); cout<<"\n";
    test2.paymentDetails(); cout<<"\n";
    test3.paymentDetails(); cout<<"\n";
    test4.paymentDetails();

    return 0;
}