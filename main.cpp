#include "game.h"
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

    delete backgroundMusic1;
    delete backgroundMusic2;

    return 0;
}
