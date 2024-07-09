#include "game.h"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    cout << "Welcome to The Snake Game!" << endl;
    cout << "Please enter your name: ";
    string name;
    getline(cin, name);
    Game game;
    game.setName(name);
    game.startGame();
}
