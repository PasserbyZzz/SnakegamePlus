#include <string>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>
#include <SFML/Audio.hpp>

#include "game.h"
#include "color.h"

using namespace std;

//构造函数
Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);
    this->mNameBoard.assign(this->mNumLeaders, "None");

    //Initialize the color
    initColors();

    //Initialize the sound
    this->pausesound = gameSound("game_start.wav");
    this->deadsound = gameSound("game_over.wav");
    this->switchsound = gameSound("light_switch.wav");
}

//析构函数
Game::~Game()
{

    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();

    delete deadsound;
    delete pausesound;
    delete switchsound;
}

//创建简介栏
void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

//显示简介
void Game::renderInformationBoard() const
{
    string name =  "Player: " + this->mName + "!";

    if (!this->mAttempt)
        mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game.");
    else
        mvwprintw(this->mWindows[0], 1, 1, "Welcome Back! Good Luck This Time.");

    mvwprintw(this->mWindows[0], 2, 1, name.c_str());
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/PasserbyZzz/SnakegamePlus");
    mvwprintw(this->mWindows[0], 4, 1, "Team Members: DZX, QJC and XKY");
    wrefresh(this->mWindows[0]);
}

//创建游戏栏
void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

//显示游戏窗口
void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

//创建提示栏
void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

//显示提示
void Game::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Points");
    mvwprintw(this->mWindows[2], 14, 1, "CD");

    wrefresh(this->mWindows[2]);
}

//显示排行榜
void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 17 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 17, 1, "Leader Board");
    std::string pointnameString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 17 - 2); i ++)
    {
        pointnameString = std::to_string(this->mLeaderBoard[i]) + " --" + this->mNameBoard[i];
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 17 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 17 + (i + 1), 5, pointnameString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

//游戏结束后，让用户选择Restart还是Quit
int Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Back", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());
    mvwprintw(menu, 2 + offset, 1, menuItems[2].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                this->switchsound->play();
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                int index_ = (index - 1 < 0) ? menuItems.size() - 1 : index - 1;
                mvwprintw(menu, index_ + offset, 1, menuItems[index_].c_str());
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                this->switchsound->play();
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                int index_ = (index + 1 > menuItems.size() - 1) ? 0 : index + 1;
                mvwprintw(menu, index_ + offset, 1, menuItems[index_].c_str());
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            this->pausesound->play();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    return index;
}

//游戏暂停后，让用户选择Continue还是Quit
int Game::renderPauseMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Continue", "Back", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Game Paused!");
    mvwprintw(menu, 2, 1, "Your Score Now:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 3, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());
    mvwprintw(menu, 2 + offset, 1, menuItems[2].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            this->switchsound->play();
            case 'W':
            case 'w':
            case KEY_UP:
            {
                this->switchsound->play();
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                int index_ = (index - 1 < 0) ? menuItems.size() - 1 : index - 1;
                mvwprintw(menu, index_ + offset, 1, menuItems[index_].c_str());
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                this->switchsound->play();
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                int index_ = (index + 1 > menuItems.size() - 1) ? 0 : index + 1;
                mvwprintw(menu, index_ + offset, 1, menuItems[index_].c_str());
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            this->pausesound->play();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    return index;
}

//修改分数，并显示在提示栏
void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

//修改难度，并显示在提示栏
void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

//修改CD时长，并显示在提示栏
void Game::renderCD() const
{
    int CD = this->mPtrSnake->getCD();
    std::string CDString;
    if (CD > 10) CDString = std::to_string(CD / 10 * 10); //数字太大的时候就不用显示个位数了，不然闪的眼睛疼
    else {
        CDString = "0"+ std::to_string(CD);
        CDString = CDString.substr(0,2); //去除末尾的0
    }
    mvwprintw(this->mWindows[2], 15, 1, CDString.c_str());
    wrefresh(this->mWindows[2]);
}

//初始化游戏界面
void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    mFood.clear();
    mScore.clear();
    mCnt.clear();
    this->createRamdonFood(this->numOfWhiteFood, this->numOfBlueFood);
    this->createRandomGate();
    this->createRandomGate();
    this->createRandomGate();
    this->mPtrSnake->senseGate(this->Gate[0], this->Gate[1], this->Gate[2], this->Gate[3], this->Gate[4], this->Gate[5]);
    this->mObstacle.clear(); //在每一轮游戏开始之前清楚上一轮游戏的所有障碍物
    this->createRandomObstacle(); //此处在函数内部就完成了sense工作，故没有再来一个sense
    this->water.clear();
    this->createRandomWater(this->numOfWaters);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mDelay = this->mBaseDelay;
}

//随机生成食物
void Game::createRamdonFood(int num1, int num2)
{
    int currentFoodnum1 = 0; int currentFoodnum2 = 0;
    while (currentFoodnum1 < num1) {
        int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;


        if (this->mPtrSnake->isPartOfSnake(x1,y1) || this->mPtrSnake->isPartOfFood(x1,y1) || this->mPtrSnake->isPartOfObstacle(x1,y1) || isPartOfWater(x1,y1)) {
            continue;
        }

        SnakeBody oneFood = SnakeBody(x1, y1);
        this->mFood.push_back(oneFood);
        this->mScore.push_back(1);
        this->mCnt.push_back(0);
        this->mPtrSnake->senseFood(oneFood, 1);

        currentFoodnum1++;
    }

    while (currentFoodnum2 < num2) {
        int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;


        if (this->mPtrSnake->isPartOfSnake(x1,y1) || this->mPtrSnake->isPartOfFood(x1,y1) || this->mPtrSnake->isPartOfObstacle(x1,y1) || isPartOfWater(x1,y1)) {
            continue;
        }

        SnakeBody oneFood = SnakeBody(x1, y1);
        this->mFood.push_back(oneFood);
        this->mScore.push_back(2);
        this->mCnt.push_back(0);
        this->mPtrSnake->senseFood(oneFood, 2);

        currentFoodnum2++;
    }
}

void Game::dispScore()
{
    for (int i = 0; i < mScore.size(); i++) {
        cout << mScore[i] << " ";
    }
    cout << endl;
}

void Game::createRandomGate()
{
    int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
    int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;
    int x2 = (rand() % (this->mGameBoardWidth - 2)) + 1;
    int y2 = (rand() % (this->mGameBoardHeight - 2)) + 1;
    while (this->mPtrSnake->isPartOfSnake(x1,y1) && this->mPtrSnake->isPartOfSnake(x2,y2) &&this->mPtrSnake->isPartOfFood(x2,y2))
    {
        x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;
        x2 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        y2 = (rand() % (this->mGameBoardHeight - 2)) + 1;
    }
    SnakeBody Gate1 = SnakeBody(x1,y1);
    SnakeBody Gate2 = SnakeBody(x2,y2);
    this->Gate.push_back(Gate1);
    this->Gate.push_back(Gate2);
}

void Game::createRandomObstacle()
{
    int currentObstaclenum = 0;
    while (currentObstaclenum < this->numOfObstacles) {
        int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;

        //生成两个叠放在一起的障碍物
        int x2 = x1 - 1;
        int y2 = y1;

        //要保证x2和y2在范围内
        if (x2 <= 0 || x2 >= this->mGameBoardWidth - 1 || y2 <= 0 || y2 >= mGameBoardHeight) {
            continue;
        }
        //保证以x1,y1为横纵坐标生成的障碍物没有重复
        if (this->mPtrSnake->isPartOfSnake(x1,y1) || this->mPtrSnake->isPartOfFood(x1,y1) || this->mPtrSnake->isPartOfObstacle(x1,y1)) {
            continue; //如果是蛇的一部分或者食物的一部分或者已经有了这个障碍物，那就需要重新生成障碍物
        }
        //保证以x2,y2为横纵坐标生成的障碍物没有重复
        if (this->mPtrSnake->isPartOfSnake(x2,y2) || this->mPtrSnake->isPartOfFood(x2,y2) || this->mPtrSnake->isPartOfObstacle(x2,y2)) {
            continue; //如果是蛇的一部分或者食物的一部分或者已经有了这个障碍物，那就需要重新生成障碍物
        }

        SnakeBody oneObstacle1 = SnakeBody(x1, y1);
        SnakeBody oneObstacle2 = SnakeBody(x2, y2);
        this->mObstacle.push_back(oneObstacle1);
        this->mObstacle.push_back(oneObstacle2);
        this->mPtrSnake->senseObstacle(oneObstacle1);
        this->mPtrSnake->senseObstacle(oneObstacle2);
        currentObstaclenum++;
    }
}

void Game::createRandomWater(int num)
{
    int currentWaternum = 0;
    while (currentWaternum < num) {
        int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;
        if (this->mPtrSnake->isPartOfSnake(x1,y1) || this->mPtrSnake->isPartOfFood(x1,y1) || this->mPtrSnake->isPartOfObstacle(x1,y1) || this->isPartOfWater(x1, y1)) {
            continue; //如果是蛇的一部分或者食物的一部分或者已经有了这个障碍物，那就需要重新生成障碍物
        }
        SnakeBody oneWater = SnakeBody(x1, y1);
        this->water.push_back(oneWater);
        currentWaternum++;
    }
}

void Game::renderFood() const
{
    for (int i = 0; i < this->mFood.size(); i++) {
        int condition = 0;
        //判断食物是否应该显示
        if (this->mCnt[i] <= 0.75 * (80 / sqrt(this->mDifficulty + 1))) {
            condition = 1;
        }

        else if (this->mCnt[i] > 0.75 * (80 / sqrt(this->mDifficulty + 1)) && this->mCnt[i] <= (80 / sqrt(this->mDifficulty + 1))) {
            if (this->mCnt[i] % 2) condition = 1;
            else condition = 2;
        }

        char mFoodSymbol;
        if (condition == 1) {
            mFoodSymbol = mFoodSymbol_first;
        }
        else {
            mFoodSymbol = mFoodSymbol_final;
        }

        //判断颜色
        if (mScore[i] == 1) {
            mvwaddch(this->mWindows[1], this->mFood[i].getY(), this->mFood[i].getX(), mFoodSymbol);
        }

        if (mScore[i] == 2) {
            wattron(this->mWindows[1], COLOR_PAIR(11));
            mvwaddch(this->mWindows[1], this->mFood[i].getY(), this->mFood[i].getX(), mFoodSymbol);
            wattroff(this->mWindows[1], COLOR_PAIR(11));
        }
    }
    wrefresh(this->mWindows[1]);
    wrefresh(this->mWindows[1]);
}

void Game::renderGate() const
{
    wattron(this->mWindows[1], COLOR_PAIR(3));
    mvwaddch(this->mWindows[1], this->Gate[0].getY(), this->Gate[0].getX(), this->mGateSymbol);
    mvwaddch(this->mWindows[1], this->Gate[1].getY(), this->Gate[1].getX(), this->mGateSymbol);
    wattroff(this->mWindows[1], COLOR_PAIR(3));

    wattron(this->mWindows[1], COLOR_PAIR(4));
    mvwaddch(this->mWindows[1], this->Gate[2].getY(), this->Gate[2].getX(), this->mGateSymbol);
    mvwaddch(this->mWindows[1], this->Gate[3].getY(), this->Gate[3].getX(), this->mGateSymbol);
    wattroff(this->mWindows[1], COLOR_PAIR(4));

    wattron(this->mWindows[1], COLOR_PAIR(5));
    mvwaddch(this->mWindows[1], this->Gate[4].getY(), this->Gate[4].getX(), this->mGateSymbol);
    mvwaddch(this->mWindows[1], this->Gate[5].getY(), this->Gate[5].getX(), this->mGateSymbol);
    wattroff(this->mWindows[1], COLOR_PAIR(5));

    wrefresh(this->mWindows[1]);
}

void Game::renderObstacle() const
{
    wattron(this->mWindows[1], COLOR_PAIR(6));
    for (SnakeBody oneObstacle: this->mObstacle) {
        mvwaddch(this->mWindows[1], oneObstacle.getY(), oneObstacle.getX(), this->mObstacleSymbol);
    }
    wattroff(this->mWindows[1], COLOR_PAIR(6));
    wrefresh(this->mWindows[1]);
}

void Game::renderWater() const
{
    wattron(this->mWindows[1], COLOR_PAIR(7));
    for (SnakeBody oneWater: this->water) {
        mvwaddch(this->mWindows[1], oneWater.getY(), oneWater.getX(), this->mFoodSymbol_first);
    }
    wattroff(this->mWindows[1], COLOR_PAIR(7));
    wrefresh(this->mWindows[1]);
}

//显示蛇
void Game::renderSnake() const {
    int snakeLength = this->mPtrSnake->getLength();
    const std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();  // 修改为const引用，避免不必要的复制

    for (int i = 0; i < snakeLength; i++) {
        //changed by qjc: 特判头部，观察
        if (this->mPtrSnake->checkInvinStatus()) {
            wattron(this->mWindows[1], COLOR_PAIR(5));  // 启用黄色
        }
        else if (i % 2 == 0) {
            wattron(this->mWindows[1], COLOR_PAIR(2));  // 启用绿色
        } else {
            wattron(this->mWindows[1], COLOR_PAIR(1));  // 启用白色
        }

        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);

        if (this->mPtrSnake->checkInvinStatus()) {
            wattroff(this->mWindows[1], COLOR_PAIR(5));  // 关闭黄色
        }
        if (i % 2 == 0) {
            wattroff(this->mWindows[1], COLOR_PAIR(2));  // 关闭绿色
        } else {
            wattroff(this->mWindows[1], COLOR_PAIR(1));  // 关闭白色
        }
    }

    wrefresh(this->mWindows[1]);
}
//玩家通过键盘控制蛇的移动方向
bool Game::controlSnake() const
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
        case 'Y':
        case 'y':
        {
            this->mPtrSnake->setInvin();
            break;
        }
        case ' ':
        {
            this->pausesound->play();
            return false;
            break;
        }
        default:
        {
            break;
        }
    }
    return true;
}

//刷新窗口 绘制三个窗口
void Game::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}

bool Game::isPartOfWater(int x, int y) const {
    SnakeBody tmp = SnakeBody(x, y);
    for (SnakeBody oneWater: water) {
        if (oneWater == tmp) return true;
    }
    return false;
}

int Game::FindIndexWater(int x, int y) const {
    SnakeBody tmp = SnakeBody(x, y);
    int index = 0;
    for (SnakeBody oneWater: water) {
        if (oneWater == tmp) return index;
        index++;
    }
    return -1;
}

//改变游戏难度，每得5分难度+1；分数被5整除的时候游戏加速
void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

//运行游戏
void Game::runGame()
{
    action:
    int suspend = 0;

    while (true)
    {
        SnakeBody head = this->mPtrSnake->getSnake()[0];
        if (isPartOfWater(head.getX(), head.getY())) {
            int index = FindIndexWater(head.getX(), head.getY());
            water.erase(water.begin()+index); //删除的蓝色食物并且添加新的蓝色食物
            this->mPtrSnake->createNewHead();
            mPoints++;
            setSlippery();
            createRandomWater(1);
        }
        dicrSlipperyTime();
        resetSlippery();

        bool x = this->controlSnake();
        if (!x)
        {
            suspend = 1;
            break;
        }
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);

        bool eatFood = this->mPtrSnake->moveFoward();
        int index(0);
        this->mPtrSnake->touchFoodIndex(index);

        bool collision = this->mPtrSnake->checkCollision();
        if (collision == true)
        {
            this->deadsound->play();
            break;
        }
       this->renderObstacle(); //changed by qjc
        this->renderWater();
        this->renderCD();
        this->renderGate();
        this->renderSnake();
        this->renderFood();
        this->renderDifficulty();
        this->renderPoints();

        if (eatFood == true)
        {
            this->mPoints += 1;

            int foodtype  = mScore[index];
            this->mScore.erase(this->mScore.begin()+index);
            this->mFood.erase(this->mFood.begin()+index);
            this->mCnt.erase(this->mCnt.begin()+index);

            if (foodtype == 2) {
                this->createRamdonFood(0,1);
                setSlippery();
            }

            else if (foodtype == 1) {
                this->createRamdonFood(1,0);
            }
            this->mPtrSnake->senseFood(this->mFood,this->mScore);
            this->adjustDelay();
        }



        //changed by qjc
        //为了模仿速度变快可以追食物的情况
        int whetherCNT = 0;
        if (whetherCNT == slippery-1) {
            for (int i = 0; i < mCnt.size(); i++) {
                mCnt[i]++;
            }
            whetherCNT = 0;
        }
        else {
            whetherCNT++;
        }

        //食物消失
        std::vector<int> checkKill;
        checkKill.clear();
        for (int i = 0; i < mCnt.size(); i++) {
            if (this->mCnt[i] > (80 / sqrt(this->mDifficulty + 1)) ) {
                int foodtype = this->mScore[i];
                checkKill.push_back(foodtype);
            }
            else {
                checkKill.push_back(0);
            }
        }
        int whiteNum = 0, blueNum = 0;
        for (int i = mCnt.size()-1; i >= 0; i--) {
            if (checkKill[i] == 1) {
                this->mScore.erase(this->mScore.begin()+i);
                this->mFood.erase(this->mFood.begin()+i);
                this->mCnt.erase(this->mCnt.begin()+i);
                whiteNum++;
            }
            if (checkKill[i] == 2) {
                this->mScore.erase(this->mScore.begin()+i);
                this->mFood.erase(this->mFood.begin()+i);
                this->mCnt.erase(this->mCnt.begin()+i);
                blueNum++;
            }
        }
        createRamdonFood(whiteNum, blueNum);
        this->mPtrSnake->senseFood(this->mFood, this->mScore);



        std::this_thread::sleep_for(std::chrono::milliseconds(int(this->mDelay/slippery))); //changed by qjc

        refresh();
    }

    if (suspend == 1)
    {
        while (true)
        {
            this->renderGate();
            this->renderSnake();
            this->renderFood();
            this->renderDifficulty();
            this->renderPoints();
            int y = this->renderPauseMenu();
            if (y == 0)
                goto action;
            else if (y == 2) {
                this->exit = true;
                break;
            }
            else {
                this->exit = true;
                this->backToMenu = true;
                break;
            }
        }
    }
}

//开始游戏
void Game::startGame()
{
    refresh();
    int choice;
    while (true)
    {
        this->readLeaderBoard();
        this->readNameBoard();
        this->renderBoards();
        this->initializeGame();
        this->runGame();
        this->updateLeaderBoard();
        this->writeLeaderBoard();
        this->writeNameBoard();
        if (this->exit) break;
        choice = this->renderRestartMenu();
        if (choice == 2)
            break;
        else if (choice == 0) {
            this->mAttempt = true;
            this->Gate.clear();
        }
        else {
            this->backToMenu = true;
            break;
        }
    }
}

//读取历史分数排行
// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i ++;
    }
    fhand.close();
    return true;
}

//读取玩家名单
bool Game::readNameBoard()
{
    std::fstream fhand(this->mNameBoardFilePath, std::ios::binary | std::ios::in);
    if (!fhand.is_open())
    {
        return false;
    }

    int i = 0;
    while (i < mNumLeaders && fhand)
    {
        size_t length;
        fhand.read(reinterpret_cast<char*>(&length), sizeof(length));
        if (fhand.eof()) break; // 防止文件末尾多读一次

        std::string temp(length, '\0');
        fhand.read(&temp[0], length);

        this->mNameBoard[i] = temp;
        i++;
    }

    fhand.close();
    return true;
}

//更新排行榜
bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    string newName = this->mName;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        string oldName = this->mNameBoard[i];
        this->mLeaderBoard[i] = newScore;
        this->mNameBoard[i] = newName;
        newScore = oldScore;
        newName = oldName;
        updated = true;
    }
    return updated;
}

//写入历史分数排行
bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}

//写入玩家名单
bool Game::writeNameBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mNameBoardFilePath, std::ios::binary | std::ios::trunc | std::ios::out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i++)
    {
        // 写入字符串长度
        size_t length = this->mNameBoard[i].size();
        fhand.write(reinterpret_cast<const char*>(&length), sizeof(length));
        // 写入字符串内容
        fhand.write(this->mNameBoard[i].c_str(), length);
    }
    fhand.close();
    return true;
}

//设置玩家姓名
void Game::setName(string name)
{
    this->mName = name;
}

bool Game::getBack() const
{
    return this->backToMenu;
}
