#ifndef CLASS_H
#define CLASS_H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

class Organism{
    protected:
        int posX=-1;
        int posY=-1;
        int breedS=3;//breed step size
        int breedC=0;//breed step count
        //type1=ant type2=poisonous ant type3=doodle
        int type=0;
    public:
        void setXY(int x, int y);
        void setType(int t);
        int getX();
        int getY();
        //type1=ant type2=poisonous ant type3=doodle
        int getType();
        int getBreedC();

        //ghostmove: predetermined movement, can get out of boundaries
        void gmove(char in);

        //random move with detection, avoids collision states
        virtual void move(int u, int d, int l, int r);

        virtual bool starve();
        static Organism * breed();
        void nBreed();
};

class Ant : public Organism{
    public:
        Ant();
        static Ant * breed();
};

class PoAnt : public Organism{
    public:
        PoAnt();
        static PoAnt * breed();
};

class Doodlebug : public Organism{
        int life=3;
    public:
        Doodlebug();
        static Doodlebug * breed();
        bool starve();
        //moves randomly decrements life, depends on checksq()
        void move(int u, int d, int l, int r);
};

class World{
        string grid;
        int sqGrid=20;
        char timer;
        vector<Organism *>bugs;
        //randomizes bug position of the last critter
        //eliminates overlaps
        //should only be used for random generation functions
        void endStackRnd();
    public:
        //random ant generation
        void addAnt(bool pois);
        void addAnt(int x, int y, bool pois);
        //random doodle generation
        void addDoodle();
        void addDoodle(int x, int y);
        int bugC();
        void diedCrit(int f);
        //checks square returns -1 when out of bounds
        //returns 0 when empty returns creature type if any
        int checkSq(int x, int y);
        int getBugRow(int x, int y);
        void printGraph();
        void breedCritts();
        void moveDoodles();
        void moveAll();

        void pesticide();
        ~World();
};

#endif