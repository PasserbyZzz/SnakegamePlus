#include "game.h"
#include "game_fun.h"
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

    bool music = false;

    action:
    //ѡ��ģʽ
    if (music)
        backgroundMusic1->play();

    int mode = renderMenu();
    sound1->play();

    if (mode == 0) {
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

        if (game.getBack()) {
            music = true;
            goto action;
        }

        delete backgroundMusic1;
        delete backgroundMusic2;
        delete sound1;
    }

    else if (mode == 1) {

    }

    else if (mode == 2) {
        //�����������
        string s = enterName();
        backgroundMusic1->stop();
        sound1->play();

        //������Ϸ�׶ε�����
        sf::Music* backgroundMusic2 = gameMusic("African_fun_long.ogg");
        backgroundMusic2->play();

        //������Ӧ��Ϸ
        GameFun game_fun;
        game_fun.setName(s);
        game_fun.startGame();
        backgroundMusic2->stop();

        if (game_fun.getBack()) {
            music = true;
            goto action;
        }

        delete backgroundMusic1;
        delete backgroundMusic2;
        delete sound1;
    }

    else if (mode == 3) {

    }

    return 0;
}
