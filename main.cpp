#include "game.h"
#include "curses.h"
#include "color.h""
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    //����׼���׶ε�����
    sf::Music* backgroundMusic1 = gameMusic("waiting.ogg");
    backgroundMusic1->play();

    //��ӡ����ͼ
    displayImageWithText();
    sf::Sound* sound1 = gameSound("game_start.wav");
    sound1->play();

    //�����������
    string s = enterName();
    backgroundMusic1->stop();
    sound1->play();

    //������Ϸ�׶ε�����
    sf::Music* backgroundMusic2 = gameMusic("African_fun_long.ogg");
    backgroundMusic2->play();

    //������Ӧ��Ϸ
    Game game;
    game.setName(s);
    game.startGame();
    backgroundMusic2->stop();

    delete backgroundMusic1;
    delete backgroundMusic2;

    return 0;
}
