#include "creature.h"

//Creature

Creature::Creature(){type=strength=hitpoints=-1;}
Creature::Creature(int newType, int newStrength, int newHit){
    type=newType; strength=newStrength; hitpoints=newHit;
}
std::string Creature::getSpecies(){
    switch(type){
        case 0: return"Human";
        case 1: return"Cyberdemon";
        case 2: return"Balrog";
        case 3: return"Elf";
    }
    return"Unknown";
}
int Creature::getType(){return type;}
int Creature::getStren(){return strength;}
int Creature::getHit(){return hitpoints;}
void Creature::setType(int i){type = i;}
void Creature::setStren(int i){strength = i;}
void Creature::setHit(int i){hitpoints = i;}
int Creature::getDamage(){
    int damage;
    // All creatures inflict damage which is a
    // random number up to their strength
    damage = (rand() % strength) + 1;
    std::cout << getSpecies() << " attacks for " <<damage << " points!\n";
    return damage;
}

    //Demon

Demon::Demon(){type=strength=hitpoints=-1;}
Demon::Demon(int newType, int newStrength, int newHit){
    type=newType; strength=newStrength; hitpoints=newHit;
}
int Demon::getDamage(){
    int damage;
    if(rand()%100 <= 5) damage =50;
    else damage = (rand() % strength) + 1;
    if(getSpecies()=="Balrog") damage*=2;
    std::cout << getSpecies() << " attacks for " <<damage << " points!\n";
    return damage;
}

        //Cyberdemon
        //Balrog
    //Elf

int Elf::getDamage(){
    int damage = (rand() % strength) + 1;
    if(rand()%100 <= 10) damage*=2;
    std::cout << getSpecies() << " attacks for " <<damage << " points!\n";
    return damage;
}
    //Human

Human::Human(){strength=10; hitpoints=10;}