#include "game.h"
#include "curses.h"
#include "color.h""
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    //��ӡ����ͼ
    displayImageWithText();

    //�����������
    string s = enterName();

    //������Ӧ��Ϸ
    Game game;
    game.setName(s);
    game.startGame();

    return 0;
}
