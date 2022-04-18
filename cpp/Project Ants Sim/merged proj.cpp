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
        void setXY(int x, int y){posX=x; posY=y;}
        void setType(int t){type=t;}
        int getX(){return posX;}
        int getY(){return posY;}
        //type1=ant type2=poisonous ant type3=doodle
        int getType(){return type;}
        int getBreedC(){return breedC;}

        //ghostmove: predetermined movement, can get out of boundaries
        void gmove(char in){
            switch (in){
                case 'l': case 'L': posX--; break;
                case 'r': case 'R': posX++; break;
                case 'd': case 'D': posY--; break;
                case 'u': case 'U': posY++; break;
                default: break;
            }
        }

        //random move with detection, avoids collision states
        virtual void move(int u, int d, int l, int r){
            switch (rand()%4){
                case 0: if(u!=0) return; ++posY; break;
                case 1: if(d!=0) return; --posY; break;
                case 2: if(l!=0) return; --posX; break;
                case 3: if(r!=0) return; ++posX; break;
                default: break;
            }
        }

        virtual bool starve(){return false;}
        static Organism * breed(){return new Organism;}
        void nBreed(){breedC=(breedC+1)%breedS;}
};

class Ant : public Organism{
    public:
        Ant(){type=1; breedS=3;}
        static Ant * breed(){return new Ant;}
};

class PoAnt : public Organism{
    public:
        PoAnt(){type=2; breedS=4;}
        static PoAnt * breed(){return new PoAnt;}
};

class Doodlebug : public Organism{
        int life=3;
    public:
        Doodlebug(){type=3; breedS=8;}
        static Doodlebug * breed(){return new Doodlebug;}
        bool starve(){return life==0;}
        //moves randomly decrements life, depends on checksq()
        void move(int u, int d, int l, int r){
            //dead mode
            if(starve()) return;

            //eat mode
            if (u==1 || u==2){gmove('u'); if (u==1)life=3;else life=2; return;}
            if (d==1 || d==2){gmove('d'); if (d==1)life=3;else life=2; return;}
            if (l==1 || l==2){gmove('l'); if (l==1)life=3;else life=2; return;}
            if (r==1 || r==2){gmove('r'); if (r==1)life=3;else life=2; return;}

            //normal mode
            switch (rand()%4){
                case 0: if(u==0) ++posY; break;
                case 1: if(d==0) --posY; break;
                case 2: if(l==0) --posX; break;
                case 3: if(r==0) ++posX; break;
                default: break;
            }
            if(life>0)life--;
        }
};

class World{
        string grid;
        int sqGrid=20;
        char timer;
        vector<Organism *>bugs;
        //randomizes bug position of the last critter
        //eliminates overlaps
        //should only be used for random generation functions
        void endStackRnd(){
            //return if no bug to randomize position
            if(bugC()==0) return;
            //return if the last bug has already been positioned (no space left to randomize)
            if(bugC()==sqGrid*sqGrid && bugs[bugC()-1]->getX()!=-1) return;

            int rndL = sqGrid*sqGrid-bugC()+1;
            int rnd = rand()%rndL;

            for (int y = 0; y < sqGrid; y++){
                for (int x = 0; x < sqGrid; x++){
                    if(checkSq(x,y)==0){
                        if(rnd==0){bugs[bugC()-1]->setXY(x,y);  return;}
                        --rnd;
                    }
                }
            }
        }
    public:
        //random ant generation
        void addAnt(bool pois){
            if(bugC() >= sqGrid*sqGrid) return;
            if(!pois && rand()%100>1)bugs.push_back(Ant::breed());
            else bugs.push_back(PoAnt::breed());
            endStackRnd();
        }
        void addAnt(int x, int y, bool pois){
            if(checkSq(x,y)!=0 || bugC() >= sqGrid*sqGrid) return;

            if(!pois && rand()%100>1)bugs.push_back(Ant::breed());
            else bugs.push_back(PoAnt::breed());

            bugs[bugC()-1]->setXY(x,y);
        }
        //random doodle generation
        void addDoodle(){
            if(bugC() >= sqGrid*sqGrid) return;
            bugs.push_back(Doodlebug::breed());
            endStackRnd();
        }
        void addDoodle(int x, int y){
            if(checkSq(x,y)!=0 || bugC() >= sqGrid*sqGrid) return;

            bugs.push_back(Doodlebug::breed());

            bugs[bugC()-1]->setXY(x,y);
        }
        int bugC(){return bugs.size();}
        void diedCrit(int f){
            if(bugC()<1 || f<0 || f>=bugC()) return;
            delete bugs[f];
            bugs.erase(bugs.begin()+f);
        }
        //checks square returns -1 when out of bounds
        //returns 0 when empty returns creature type if any
        int checkSq(int x, int y){
            if(x<0 || y<0 || x>=sqGrid || y>=sqGrid) return -1;
            for (int i = 0; i < bugC(); i++)
                if(bugs[i]->getX()==x && bugs[i]->getY()==y)
                    return bugs[i]->getType();
            return 0;
        }
        int getBugRow(int x, int y){
            if(x<0 || y<0 || x>sqGrid-1 || y>sqGrid-1)return -1;
            for (int i = 0; i < bugC(); i++)
                if(bugs[i]->getX()==x && bugs[i]->getY()==y)
                    return i;
            return -1;
        }
        void printGraph(){
            //top border
            cout<<" \u2554";
            for (int i = 0; i < sqGrid*3; i++) cout<<"\u2550";
            cout<<"\u2557\n";

            //mid borders and ground
            for (int i = sqGrid-1; i >= 0; i--){
                cout<<" \u2551";
                    for (int j = 0; j < sqGrid; j++){
                        switch (checkSq(j,i)){
                            case 1: cout<<" \u25cb "; break;
                            case 2: cout<<" \u24b8 "; break;
                            case 3: cout<<" \u2573 "; break;
                            default: cout<<"   "; break;
                        }
                    }
                cout<<"\u2551\n";
            }

            //bottom border
            cout<<" \u255a";
            for (int i = 0; i < sqGrid*3; i++) cout<<"\u2550";
            cout<<"\u255d\n";
        }
        void breedCritts(){
            int x,y,bC=bugC();
            //bug count changes with every replication
            //we need to take the static value
            for (int i = 0; i < bC; i++){
                bugs[i]->nBreed();
                if(bugs[i]->getBreedC()==0){
                    x=bugs[i]->getX(); y=bugs[i]->getY();
                    switch (rand()%4){
                        case 0: if(checkSq(x,++y)!=0) goto breedskip; break;
                        case 1: if(checkSq(x,--y)!=0) goto breedskip; break;
                        case 2: if(checkSq(--x,y)!=0) goto breedskip; break;
                        case 3: if(checkSq(++x,y)!=0) goto breedskip; break;
                    }
                    switch (bugs[i]->getType()){
                        case 1: addAnt(x,y,false); break;
                        case 2: addAnt(x,y,true); break;
                        case 3: addDoodle(x,y); break;
                        default: break;
                    }
                }
                breedskip:;
            }
        }
        void moveDoodles(){
            for (int i = 0; i < bugC(); i++){
                if (bugs[i]->getType() == 3){
                    if(!bugs[i]->starve()){
                        int x=bugs[i]->getX(), y=bugs[i]->getY(),
                        tU=checkSq(x,y+1),tD=checkSq(x,y-1),tL=checkSq(x-1,y),tR=checkSq(x+1,y);
                        int target=0, a;

                        if(tU==1 || tU==2) {target=1; goto eatit;}
                        if(tD==1 || tD==2) {target=2; goto eatit;}
                        if(tL==1 || tL==2) {target=3; goto eatit;}
                        if(tR==1 || tR==2) {target=4; goto eatit;}

                        if(target==0) goto noprey;

                        eatit:
                        switch (target){
                            case 1: a=getBugRow(x,y+1); break;
                            case 2: a=getBugRow(x,y-1); break;
                            case 3: a=getBugRow(x-1,y); break;
                            case 4: a=getBugRow(x+1,y); break;
                        }
                        diedCrit(a);
                        if (a<i) --i;
                        noprey:
                        bugs[i]->move(tU,tD,tL,tR);
                    }
                    else{
                        diedCrit(i); --i;
                    }
                }
            }
        }
        void moveAll(){
            //predators first
            moveDoodles();
            for (int i = 0; i < bugC(); i++){
                if (bugs[i]->getType() != 3){
                    int x=bugs[i]->getX(), y=bugs[i]->getY();
                    bugs[i]->move(
                        checkSq(x, y+1),
                        checkSq(x, y-1),
                        checkSq(x-1, y),
                        checkSq(x+1, y)
                    );
                }
            }
            breedCritts();
        }

        void pesticide(){
            for (int i = 0; i < bugC(); i++)
                delete bugs[i];
            bugs.clear();
        }
        ~World(){pesticide();}
};

int main(){
    srand(time(NULL));
    World test;

    for (int i = 0; i < 100; i++) test.addAnt(false);
    for (int i = 0; i < 3; i++) test.addDoodle();
    system("clear");
    test.printGraph();
    cout<<test.bugC()<<endl;
    for (int i = 0; i < 1000; i++){
        sleep(1);
        system("clear");
        test.moveAll();
        test.printGraph();
        cout<<test.bugC()<<endl;
    }
    return 0;
}