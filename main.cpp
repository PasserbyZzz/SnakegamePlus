#include "game.h"
#include "curses.h"
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    //输入玩家姓名
    initscr(); //初始化curses模式
    raw();
    curs_set(0);
    char* welcome = "Welcome to The Snake Game!";
    mvprintw(LINES / 2 - 1, (COLS - strlen(welcome)) / 2, welcome);
    char* player = "Please enter your name: ";
    mvprintw(LINES / 2, (COLS - strlen(player)) / 2, player);
    refresh();
    char name[7];
    mvgetstr(LINES / 2, (COLS - strlen(player)) / 2 + strlen(player), name); //玩家输入姓名
    string s(name);
    endwin(); //结束curses模式

    Game game;
    game.setName(s);
    game.startGame();
}
