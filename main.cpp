#include "game.h"
#include "curses.h"
#include "color.h""
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    //打印背景图
    displayImageWithText();

    //输入玩家姓名
    string s = enterName();

    //生成相应游戏
    Game game;
    game.setName(s);
    game.startGame();

    return 0;
}
