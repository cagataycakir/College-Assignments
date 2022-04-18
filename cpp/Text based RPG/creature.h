#ifndef CREATURE_H
#define CREATURE_H
#include <string>
#include <iostream>
#include <time.h>

class Creature{
    protected:
        //a member data which defines the type
        int type;
        //a member data which stores the strength
        int strength;
        //a member data which stores the hitpoints
        int hitpoints;

    public:
        Creature();
        // Initialize creature to new type, strength, hit points
        Creature(int newType, int newStrength, int newHit);
        // Appropriate accessor and mutator functions
        std::string getSpecies();
        int getType();
        int getStren();
        int getHit();
        void setType(int i);
        void setStren(int i);
        void setHit(int i);

        // Returns amount of damage this creature
        // inflicts in one round of combat
        virtual int getDamage();
};

class Demon : public Creature{
    public:
        Demon();
        Demon(int newType, int newStrength, int newHit);
        // Demons can inflict damage of 50 with a 5% chance
        // Balrogs are so fast they get to attack twice
        int getDamage() override;
};

class Cyberdemon : public Demon{
    public:
        using Demon::Demon;
};

class Balrog : public Demon{
    public:
        using Demon::Demon;
};

class Elf : public Creature{
    public:
        using Creature::Creature;
        // Elves inflict double magical damage with a 10% chance
        int getDamage() override;
};

class Human : public Creature{
    public:
        using Creature::Creature;
        // Initialize to human, 10 strength, 10 hit points
        Human();
};

#endif