#include <iostream>

using namespace std;

class Player{
    public:
        virtual int getGuess(){return 0;}
};

class HumanPlayer : public Player{
    public:
        int getGuess(){
            int a;
            cin>>a;
            return a;
        }
};

class ComputerPlayer : public Player{
    public:
        int getGuess(){
            int a = rand()%100;
            cout<<"Computer guesses: "<<a<<"\n";
            return a;
        }
};

bool checkForWin(int guess, int answer){
    if (answer == guess){
        cout << "You're right! You win!" << endl;
        return true;
    }else if (answer < guess)
        cout << "Your guess is too high." << endl;
    else
        cout << "Your guess is too low." << endl;
    return false;
}

void play(Player &player1, Player &player2){
    int answer = 0, guess = 0;
    answer = rand( ) % 100;
    bool win = false;
    while (!win){cout << "Player 1's turn to guess." << endl;
        guess = player1.getGuess( );
        win = checkForWin(guess, answer);
        if (win) return;

        cout << "Player 2's turn to guess." << endl;
        guess = player2.getGuess( );
        win = checkForWin(guess, answer);
    }
}

int main(){
    srand(time(NULL));
    HumanPlayer player1,player2;
    ComputerPlayer comp;

    play(player1,  player2);
    play(player1,  comp);

    return 0;
}