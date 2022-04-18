#include <iostream>
#include <string>

using namespace std;

class Person{
    public:
        Person(){}
        Person(string theName){name=theName;}
        Person(const Person& theObject){name=theObject.name;}
        string getName() const {return name;}
        Person& operator=(const Person& rtSide){
            name=rtSide.name;
        }
        Person& operator=(string newN){
            name=newN;
        }
        friend istream& operator >>(istream& inStream,Person& personObject){
            inStream>>personObject.name;
            return inStream;
        }
        friend ostream& operator <<(ostream& outStream,const Person& personObject){
            outStream<<personObject.name;
            return outStream;
        }
    private:
        string name;
};

class Vehicle{
    public:
        Vehicle(){}
        Vehicle(int cyl, string manu, string own){
            cylinders=cyl; manufacturer=manu, owner=own;
        }
        friend ostream& operator <<(ostream& outS,const Vehicle& myVe){
            outS<<"Owner: "<<myVe.owner<<"\nManufacturer: "<<myVe.manufacturer
            <<"\nCylinders: "<<myVe.cylinders;
            return outS;
        }
        Vehicle& operator=(const Vehicle& other){
            *this=other;
        }
        Vehicle& operator=(const Person& own){
            owner=own;
        }
        Vehicle& operator=(string manuf){
            manufacturer=manuf;
        }
        int getCy(){return cylinders;}
        string getMan(){return manufacturer;}
        Person getOwn(){return owner;}
        void setCy(int in){cylinders=in;}
        void setMan(string in){manufacturer=in;}
        void setOwn(string in){owner=in;}
        void setOwn(Person in){owner=in;}
    protected:
        string manufacturer;
        int cylinders;
        Person owner;
};

class Truck: public Vehicle{
    public:
        Truck(){}
        Truck(int cyl, int load, double tow, string manu, string own){
            cylinders=cyl; loadCap=load; towCap=tow;
            manufacturer=manu; owner=own; 
        }
        Truck(Vehicle cop ,int load, double tow){
            (*this)=cop;
            loadCap=load;
            towCap=tow;
        }
        friend ostream& operator <<(ostream& outS,const Truck& myTr){
            outS<<"Owner: "<<myTr.owner<<"\nManufacturer: "<<myTr.manufacturer
            <<"\nCylinders: "<<myTr.cylinders<<"\nLoad Capacity: "<<myTr.loadCap
            <<"\nTow Capacity: "<<myTr.towCap;
            return outS;
        }
        Truck& operator=(const Person& own){
            owner=own;
        }
        Truck& operator=(Vehicle& rVec){
            cylinders=rVec.getCy();
            manufacturer=rVec.getMan();
            owner=rVec.getOwn();
        }
        int getTow(){return towCap;}
        double getLoad(){return loadCap;}
        void setLoad(double in){loadCap=in;}
        void setTow(int in){towCap=in;}
    private:
        double loadCap;
        int towCap;
};

int main(){
    Truck testTruck, testTruck1(40,10.4,14,"Tobota","Michael");

    Vehicle testVehicle; testVehicle="Mort"; testVehicle.setCy(5);
    Person testPerson; testPerson="John"; testVehicle=testPerson;

    testTruck=testVehicle;
    testTruck.setOwn("John");
    testTruck.setCy(20);
    testTruck.setLoad(1.2);
    testTruck.setTow(10);

    cout<<testTruck<<"\n\n";
    cout<<testVehicle<<"\n\n";
    cout<<testTruck1<<"\n";
    return 0;
}