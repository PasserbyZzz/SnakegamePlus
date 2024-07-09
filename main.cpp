#include "game.h"
#include "curses.h"
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    //�����������
    initscr(); //��ʼ��cursesģʽ
    raw();
    curs_set(0);
    char* welcome = "Welcome to The Snake Game!";
    mvprintw(LINES / 2 - 1, (COLS - strlen(welcome)) / 2, welcome);
    char* player = "Please enter your name: ";
    mvprintw(LINES / 2, (COLS - strlen(player)) / 2, player);
    refresh();
    char name[7];
    mvgetstr(LINES / 2, (COLS - strlen(player)) / 2 + strlen(player), name); //�����������
    string s(name);
    endwin(); //����cursesģʽ

    Game game;
    game.setName(s);
    game.startGame();
}
