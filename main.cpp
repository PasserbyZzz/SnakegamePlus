#include "game.h"
#include "game_fun.h"
#include "game_pair.h"
#include "curses.h"
#include "color.h""
#include <string>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char** argv)
{
    //播放准备阶段的音乐
    sf::Music* backgroundMusic1 = gameMusic("waiting.ogg");
    backgroundMusic1->play();

    //打印背景图
    displayImageWithText();
    sf::Sound* sound1 = gameSound("game_start.wav");
    sound1->play();

    bool music = false;

    action:
    //选择模式
    if (music)
        backgroundMusic1->play();

    int mode = renderMenu();
    sound1->play();

    if (mode == 0) {
        //输入玩家姓名
        string s = enterName();
        backgroundMusic1->stop();
        sound1->play();

        //播放游戏阶段的音乐
        sf::Music* backgroundMusic2 = gameMusic("African_fun_long.ogg");
        backgroundMusic2->play();

        //生成相应游戏
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
        string s1;
        string s2;
        enterNamePair(s1, s2);
        backgroundMusic1->stop();
        sound1->play();

        sf::Music* backgroundMusic2 = gameMusic("African_fun_long.ogg");
        backgroundMusic2->play();

        GamePair game_pair;
        game_pair.setName(s1,s2);
        game_pair.startGame();
        backgroundMusic2->stop();

        if (game_pair.getBack()) {
            music = true;
            goto action;
        }

        delete backgroundMusic1;
        delete backgroundMusic2;
        delete sound1;
    }

    else if (mode == 2) {
        //输入玩家姓名
        string s = enterName();
        backgroundMusic1->stop();
        sound1->play();

        //播放游戏阶段的音乐
        sf::Music* backgroundMusic2 = gameMusic("African_fun_long.ogg");
        backgroundMusic2->play();

        //生成相应游戏
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
