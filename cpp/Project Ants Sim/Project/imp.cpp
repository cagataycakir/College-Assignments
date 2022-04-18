#include "header.h"

using namespace std;

void Organism::setXY(int x, int y){posX=x; posY=y;}
void Organism::setType(int t){type=t;}
int Organism::getX(){return posX;}
int Organism::getY(){return posY;}
int Organism::getType(){return type;}
int Organism::getBreedC(){return breedC;}
void Organism::gmove(char in){
    switch (in){
        case 'l': case 'L': posX--; break;
        case 'r': case 'R': posX++; break;
        case 'd': case 'D': posY--; break;
        case 'u': case 'U': posY++; break;
        default: break;
    }
}
void Organism::move(int u, int d, int l, int r){
    switch (rand()%4){
        case 0: if(u!=0) return; ++posY; break;
        case 1: if(d!=0) return; --posY; break;
        case 2: if(l!=0) return; --posX; break;
        case 3: if(r!=0) return; ++posX; break;
        default: break;
    }
}
bool Organism::starve(){return false;}
Organism * Organism::breed(){return new Organism;}
void Organism::nBreed(){breedC=(breedC+1)%breedS;}

Ant::Ant(){type=1; breedS=3;}
Ant * Ant::breed(){return new Ant;}

PoAnt::PoAnt(){type=2; breedS=4;}
PoAnt * PoAnt::breed(){return new PoAnt;}

Doodlebug::Doodlebug(){type=3; breedS=8;}
Doodlebug * Doodlebug::breed(){return new Doodlebug;}
bool Doodlebug::starve(){return life==0;}
void Doodlebug::move(int u, int d, int l, int r){
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

void World::endStackRnd(){
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
void World::addAnt(bool pois){
    if(bugC() >= sqGrid*sqGrid) return;
    if(!pois && rand()%100>1)bugs.push_back(Ant::breed());
    else bugs.push_back(PoAnt::breed());
    endStackRnd();
}
void World::addAnt(int x, int y, bool pois){
    if(checkSq(x,y)!=0 || bugC() >= sqGrid*sqGrid) return;

    if(!pois && rand()%100>1)bugs.push_back(Ant::breed());
    else bugs.push_back(PoAnt::breed());

    bugs[bugC()-1]->setXY(x,y);
}
void World::addDoodle(){
    if(bugC() >= sqGrid*sqGrid) return;
    bugs.push_back(Doodlebug::breed());
    endStackRnd();
}
void World::addDoodle(int x, int y){
    if(checkSq(x,y)!=0 || bugC() >= sqGrid*sqGrid) return;

    bugs.push_back(Doodlebug::breed());

    bugs[bugC()-1]->setXY(x,y);
}
int World::bugC(){return bugs.size();}
void World::diedCrit(int f){
    if(bugC()<1 || f<0 || f>=bugC()) return;
    delete bugs[f];
    bugs.erase(bugs.begin()+f);
}
int World::checkSq(int x, int y){
    if(x<0 || y<0 || x>=sqGrid || y>=sqGrid) return -1;
    for (int i = 0; i < bugC(); i++)
        if(bugs[i]->getX()==x && bugs[i]->getY()==y)
            return bugs[i]->getType();
    return 0;
}
int World::getBugRow(int x, int y){
    if(x<0 || y<0 || x>sqGrid-1 || y>sqGrid-1)return -1;
    for (int i = 0; i < bugC(); i++)
        if(bugs[i]->getX()==x && bugs[i]->getY()==y)
            return i;
    return -1;
}
void World::printGraph(){
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
void World::breedCritts(){
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
void World::moveDoodles(){
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
void World::moveAll(){
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
void World::pesticide(){
    for (int i = 0; i < bugC(); i++)
        delete bugs[i];
    bugs.clear();
}
World::~World(){pesticide();}