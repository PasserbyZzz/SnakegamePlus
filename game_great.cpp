#include <string>
#include <iostream>
#include <cmath>
#include <string>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>
#include <SFML/Audio.hpp>

#include "game_great.h"
#include "color.h"

using namespace std;

//���캯��
Game_great::Game_great()
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

    initColors();

    //Initialize the sound
    this->pausesound = gameSound("game_start.wav");
    this->deadsound = gameSound("game_over.wav");
    this->switchsound = gameSound("light_switch.wav");
    this->dingsound = gameSound("ding.wav");
}

//��������
Game_great::~Game_great()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
    delete pausesound;
    delete deadsound;
    delete switchsound;
    delete dingsound;
}

//���������
void Game_great::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

//��ʾ���
void Game_great::renderInformationBoard() const
{
    string name =  "Player: " + this->mName + "!"; //changed by qjc

    if (!this->mAttempt)
        mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game.");
    else
        mvwprintw(this->mWindows[0], 1, 1, "Welcome Back! Good Luck This Time.");

    mvwprintw(this->mWindows[0], 2, 1,  name.c_str());
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/PasserbyZzz/SnakegamePlus");
    mvwprintw(this->mWindows[0], 4, 1, "Team Members: DZX, QJC and XKY");
    wrefresh(this->mWindows[0]);
}

//������Ϸ��
void Game_great::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

//��ʾ��Ϸ����
void Game_great::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

//������ʾ��
void Game_great::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

//��ʾ��ʾ
void Game_great::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "MAGENTA: 1");
    mvwprintw(this->mWindows[2], 4, 1, "RED    : 2");
    mvwprintw(this->mWindows[2], 5, 1, "YELLOW : 3");
    mvwprintw(this->mWindows[2], 6, 1, "BLUE   : 5");

    mvwprintw(this->mWindows[2], 8, 1, "Stage");
    mvwprintw(this->mWindows[2], 11, 1, "Points");
    mvwprintw(this->mWindows[2], 14, 1, "Steps Left"); //changed by qjc
    mvwprintw(this->mWindows[2], 14, 13, "Len");

    wrefresh(this->mWindows[2]);
    wrefresh(this->mWindows[0]);
}

//changed by qjc�� �����а��λ�ô�14�ĵ���17�����Ұ�this->this->mScreenHeight - this->mInformationHeight - 17 -2�е�-2ɾ��
//��ʾ���а�
void Game_great::renderLeaderBoard() const
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

//��Ϸ���������û�ѡ��Restart����Quit
int Game_great::renderRestartMenu() const
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

//��Ϸ��ͣ�����û�ѡ��Continue����Quit
int Game_great::renderPauseMenu() const
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

//�޸ķ���������ʾ����ʾ��
void Game_great::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints) + "          ";
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

//�޸��Ѷȣ�����ʾ����ʾ��
void Game_great::renderDifficulty() const
{
    std::string recremode("Gain  ");
    if (!gainorlose) {
        recremode = "Lose  ";
    }
    std::string difficultyString = recremode + std::to_string(this->mDifficulty + 1);
    mvwprintw(this->mWindows[2], 9, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

//changed by qjc
//�޸ĳ���
void Game_great::renderCD() const
{
    int CD(0);
    if (this->mCnt > oneStageTime/2) {
        CD = oneStageTime - this->mCnt;
    }
    else {
        CD = oneStageTime/2 - this->mCnt;
    }

    std::string CDString;
    if (CD > 10) CDString = std::to_string(CD / 10 * 10) + "  "; //����̫���ʱ��Ͳ�����ʾ��λ���ˣ���Ȼ�����۾���
    else {
        CDString = std::to_string(CD)+ "  ";
    }
    mvwprintw(this->mWindows[2], 15, 1, CDString.c_str());

    int len = this->mPtrSnake->getLength();
    std::string lenString;
    lenString = std::to_string(len)+ "  ";
    mvwprintw(this->mWindows[2], 15, 13, lenString.c_str());
    wrefresh(this->mWindows[2]);
}

//��ʼ����Ϸ����
void Game_great::initializeGame()
{
    this->mPtrSnake.reset(new Snake_great(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->mFood.clear();  //�����һ�ֵ�ʳ��
    this->mScore.clear();
    this->createRamdonFood();

    this->createRandomGate();
    this->createRandomGate();
    this->createRandomGate();
    this->mPtrSnake->senseGate(this->Gate[0], this->Gate[1], this->Gate[2], this->Gate[3], this->Gate[4], this->Gate[5]);
    this->mObstacle.clear(); //changed by qjc ��ÿһ����Ϸ��ʼ֮ǰ�����һ����Ϸ�������ϰ���
    this->createRandomObstacle(); //changed by qjc �˴��ں����ڲ��������sense��������û������һ��sense
    this->water.clear();
    this->createRandomWater(this->numOfWaters);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mCnt = 0;
    this->mDelay = this->mBaseDelay;
}

//�������ʳ��ķ�ֵ��0.5������Ϊ1,0.3������Ϊ2,0.15������Ϊ3,0.05������Ϊ5
int Game_great::generateRandomNumber() {
    double randValue = (double)rand() / RAND_MAX; // ����һ��0��1֮��������
    if (randValue < 0.5) {
        return 1;
    } else if (randValue < 0.8) {
        return 2;
    } else if (randValue < 0.95) {
        return 3;
    } else {
        return 5;
    }
}

//�������ʳ��
void Game_great::createRamdonFood()
{
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            //���ɸ���Ϊ0.125
            int whethertodropout = rand() % 15;


            if (whethertodropout) {
                continue;
            }
            if (j == this->mGameBoardWidth/2 && i == this->mGameBoardHeight/2) continue;

            //changed by qjc
            if(this->mPtrSnake->isPartOfSnake(j, i) || this->mPtrSnake->isPartOfFood(j, i) || this->mPtrSnake->isPartOfObstacle(j, i) || isPartOfWater(j, i))
            {
                continue;
            }

            else
            {
                int random_score = generateRandomNumber();
                SnakeBody_great Food = SnakeBody_great(j,i);
                this->mFood.push_back(Food);
                this->mScore.push_back(random_score);
                this->mPtrSnake->senseFood(Food, random_score);
            }
        }
    }
}

//�������ʳ�ָ������
void Game_great::createRamdonFood(int num)
{
    int currentFoodnum = 0;
    while (currentFoodnum < num) {
        int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;


        //��֤��x1,y1Ϊ�����������ɵ��ϰ���û���ظ�
        if (this->mPtrSnake->isPartOfSnake(x1,y1) || this->mPtrSnake->isPartOfFood(x1,y1) || this->mPtrSnake->isPartOfObstacle(x1,y1) || isPartOfWater(x1,y1)) {
            continue; //������ߵ�һ���ֻ���ʳ���һ���ֻ����Ѿ���������ϰ���Ǿ���Ҫ���������ϰ���
        }

        int random_score = generateRandomNumber();
        SnakeBody_great oneFood = SnakeBody_great(x1, y1);
        this->mFood.push_back(oneFood);
        this->mScore.push_back(random_score);
        this->mPtrSnake->senseFood(oneFood, random_score);

        currentFoodnum++;
    }
}

//������ɴ�����
void Game_great::createRandomGate()
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
    SnakeBody_great Gate1 = SnakeBody_great(x1,y1);
    SnakeBody_great Gate2 = SnakeBody_great(x2,y2);
    this->Gate.push_back(Gate1);
    this->Gate.push_back(Gate2);
}

//��������ϰ�����ں����ڲ���������߸�֪���ϰ��������
//changed by qjc��
void Game_great::createRandomObstacle()
{
    int currentObstaclenum = 0;
    while (currentObstaclenum < this->numOfObstacles) {
        int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;

        //��������������һ����ϰ���
        int x2 = x1 - 1;
        int y2 = y1;

        //Ҫ��֤x2��y2�ڷ�Χ��
        if (x2 <= 0 || x2 >= this->mGameBoardWidth - 1 || y2 <= 0 || y2 >= mGameBoardHeight) {
            continue;
        }
        //��֤��x1,y1Ϊ�����������ɵ��ϰ���û���ظ�
        if (this->mPtrSnake->isPartOfSnake(x1,y1) || this->mPtrSnake->isPartOfFood(x1,y1) || this->mPtrSnake->isPartOfObstacle(x1,y1)) {
            continue; //������ߵ�һ���ֻ���ʳ���һ���ֻ����Ѿ���������ϰ���Ǿ���Ҫ���������ϰ���
        }
        //��֤��x2,y2Ϊ�����������ɵ��ϰ���û���ظ�
        if (this->mPtrSnake->isPartOfSnake(x2,y2) || this->mPtrSnake->isPartOfFood(x2,y2) || this->mPtrSnake->isPartOfObstacle(x2,y2)) {
            continue; //������ߵ�һ���ֻ���ʳ���һ���ֻ����Ѿ���������ϰ���Ǿ���Ҫ���������ϰ���
        }

        SnakeBody_great oneObstacle1 = SnakeBody_great(x1, y1);
        SnakeBody_great oneObstacle2 = SnakeBody_great(x2, y2);
        this->mObstacle.push_back(oneObstacle1);
        this->mObstacle.push_back(oneObstacle2);
        this->mPtrSnake->senseObstacle(oneObstacle1);
        this->mPtrSnake->senseObstacle(oneObstacle2);
        currentObstaclenum++;
    }
}


//������ˮ��ò
//changed by qjc
void Game_great::createRandomWater(int num)
{
    int currentWaternum = 0;
    while (currentWaternum < num) {
        int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;
        if (this->mPtrSnake->isPartOfSnake(x1,y1) || this->mPtrSnake->isPartOfFood(x1,y1) || this->mPtrSnake->isPartOfObstacle(x1,y1) || this->isPartOfWater(x1, y1)) {
            continue; //������ߵ�һ���ֻ���ʳ���һ���ֻ����Ѿ���������ϰ���Ǿ���Ҫ���������ϰ���
        }
        SnakeBody_great oneWater = SnakeBody_great(x1, y1);
        this->water.push_back(oneWater);
        currentWaternum++;
    }
}

//��ʾʳ��
void Game_great::renderFood_first() const
{
    int change(0);
    if (!gainorlose) {
        change = 4;
    }

    for (int i = 0; i < this->mFood.size(); i++) {
        if (mScore[i] == 1) {
            wattron(this->mWindows[1], COLOR_PAIR(8+change));
            mvwaddch(this->mWindows[1], this->mFood[i].getY(), this->mFood[i].getX(), mFoodSymbol_first);
            wattroff(this->mWindows[1], COLOR_PAIR(8+change));
        }

        if (mScore[i] == 2) {
            wattron(this->mWindows[1], COLOR_PAIR(9+change));
            mvwaddch(this->mWindows[1], this->mFood[i].getY(), this->mFood[i].getX(), mFoodSymbol_first);
            wattroff(this->mWindows[1], COLOR_PAIR(9+change));
        }

        if (mScore[i] == 3) {
            wattron(this->mWindows[1], COLOR_PAIR(10+change));
            mvwaddch(this->mWindows[1], this->mFood[i].getY(), this->mFood[i].getX(), mFoodSymbol_first);
            wattroff(this->mWindows[1], COLOR_PAIR(10+change));
        }

        if (mScore[i] == 5) {
            wattron(this->mWindows[1], COLOR_PAIR(11+change));
            mvwaddch(this->mWindows[1], this->mFood[i].getY(), this->mFood[i].getX(), mFoodSymbol_first);
            wattroff(this->mWindows[1], COLOR_PAIR(11+change));
        }
    }
    wrefresh(this->mWindows[1]);
}


//��ʾ������
void Game_great::renderGate() const
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

//��ʾ�ϰ���
//changed by qjc
void Game_great::renderObstacle() const
{
    wattron(this->mWindows[1], COLOR_PAIR(6));
    for (SnakeBody_great oneObstacle: this->mObstacle) {
        mvwaddch(this->mWindows[1], oneObstacle.getY(), oneObstacle.getX(), this->mObstacleSymbol);
    }
    wattroff(this->mWindows[1], COLOR_PAIR(6));
    wrefresh(this->mWindows[1]);
}

void Game_great::renderWater() const
{
    wattron(this->mWindows[1], COLOR_PAIR(7));
    for (SnakeBody_great oneWater: this->water) {
        mvwaddch(this->mWindows[1], oneWater.getY(), oneWater.getX(), this->mWaterSymbol);
    }
    wattroff(this->mWindows[1], COLOR_PAIR(7));
    wrefresh(this->mWindows[1]);
}

//��ʾ��
void Game_great::renderSnake() const {
    int snakeLength = this->mPtrSnake->getLength();
    const std::vector<SnakeBody_great>& snake = this->mPtrSnake->getSnake();  // �޸�Ϊconst���ã����ⲻ��Ҫ�ĸ���

    std::string lenString = std::to_string(this->mPtrSnake->getLength())+ " ";
    mvwprintw(this->mWindows[1],this->mGameBoardHeight/2,  this->mGameBoardWidth/2 , lenString.c_str());

    for (int i = 0; i < snakeLength; i++) {
        //changed by qjc: ����ͷ�����۲�
        if (this->mPtrSnake->checkInvinStatus()) {
            wattron(this->mWindows[1], COLOR_PAIR(5));  // ���û�ɫ
        }
        else if (i % 2 == 0) {
            wattron(this->mWindows[1], COLOR_PAIR(2));  // ������ɫ
        } else {
            wattron(this->mWindows[1], COLOR_PAIR(1));  // ���ð�ɫ
        }

        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);

        if (this->mPtrSnake->checkInvinStatus()) {
            wattroff(this->mWindows[1], COLOR_PAIR(5));  // �رջ�ɫ
        }
        if (i % 2 == 0) {
            wattroff(this->mWindows[1], COLOR_PAIR(2));  // �ر���ɫ
        } else {
            wattroff(this->mWindows[1], COLOR_PAIR(1));  // �رհ�ɫ
        }
    }

    wrefresh(this->mWindows[1]);
}

//���ͨ�����̿����ߵ��ƶ�����
bool Game_great::controlSnake()
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction_great::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction_great::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction_great::Left);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction_great::Right);
            break;
        }
        case 'Y':
        case 'y':
        {
            this->mPtrSnake->setInvin(); //changed by qjc
            break;
        }
        case 'H':
        case 'h':
        {
            this->dingsound->play();
            this->setSlippery();
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

//ˢ�´��� ������������
void Game_great::renderBoards() const
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


//�ж��Ƿ���ˮ��һ����
bool Game_great::isPartOfWater(int x, int y) const {
    SnakeBody_great tmp = SnakeBody_great(x, y);
    for (SnakeBody_great oneWater: water) {
        if (oneWater == tmp) return true;
    }
    return false;
}

int Game_great::FindIndexWater(int x, int y) const {
    SnakeBody_great tmp = SnakeBody_great(x, y);
    int index = 0;
    for (SnakeBody_great oneWater: water) {
        if (oneWater == tmp) return index;
        index++;
    }
    return -1;
}

//�ı���Ϸ�Ѷȣ������ж�
bool Game_great::adjustDelay()
{
    if (this->mCnt == this->oneStageTime/2) {
        gainorlose = false;
        if (this->mPtrSnake->getLength() < 15 ) return true;
    }

    if (this->mCnt == this->oneStageTime) {
        gainPointsInOneStage = 0;
        mDifficulty++;
        this->mDelay = this->mBaseDelay * pow(0.95, this->mDifficulty);
        gainorlose = true;
        this->mCnt = 0;
        if (this->mPtrSnake->getLength() > 15 ) return true;
    }
    return false;
}

//������Ϸ
void Game_great::runGame()
{
    action:
    gainorlose = true;
    int suspend = 0;
    mCnt = 0;
    while (true)
    {
        //ʧ�ֽ׶Σ�+1
        //�÷ֽ׶Σ�-1
        //bonus�������ʧ�ֽ׶γ���15���ϰ������ϣ����Ի�õ÷ֽ׶ε�2������
        int recreationMode;
        int loseNum;

        if (gainorlose) {
            recreationMode = 1;
            loseNum = 0;
        }
        else {
            recreationMode = -1;
        }


        //����״̬����
        //changed by qjc
        SnakeBody_great head = this->mPtrSnake->getSnake()[0];
        if (isPartOfWater(head.getX(), head.getY())) {
            int index = FindIndexWater(head.getX(), head.getY());
            water.erase(water.begin()+index); //ɾ�����֮ǰ��ˮ�����������µ�ˮ��
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

        if (eatFood == true)
        {
            //ʧ�ֽ׶���������
            if (!gainorlose) {
                this->mPtrSnake->getSnake().pop_back();
                this->mPtrSnake->getSnake().pop_back();
                loseNum++;
                if (loseNum > 20) {
                    loseNum = 0;
                    mPoints += gainPointsInOneStage*3;

                }
            }

            if (gainorlose) {
                gainPointsInOneStage += mScore[index];
            }

            this->mPoints += mScore[index]*recreationMode; //ע���ǳԵ��ĸ�ʳ�����Щ����
            this->mScore.erase(this->mScore.begin()+index);
            this->mFood.erase(this->mFood.begin()+index);
            this->createRamdonFood(1);
            this->mPtrSnake->senseFood(mFood, mScore);


        }
        this->renderObstacle(); //changed by qjc
        this->renderWater();
        this->renderCD();
        this->renderGate();
        this->renderSnake();
        //ʳ���Ƕ��ܰ�ȫ
        this->renderFood_first();
        this->renderDifficulty();
        this->renderPoints();



        //changed by qjc
        //Ϊ��ģ���ٶȱ�����׷ʳ������
        int whetherCNT = 0;
        if (whetherCNT == slippery-1) {
            this->mCnt++;
            whetherCNT = 0;
        }
        else {
            whetherCNT++;
        }

        if (this->adjustDelay()) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(int(mDelay/slippery))); //changed by qjc

        refresh();
    }

    if (suspend == 1)
    {
        while (true)
        {
            this->renderGate();
            this->renderSnake();
            this->renderFood_first();
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

//��ʼ��Ϸ
void Game_great::startGame()
{   refresh();
    bool choice;
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
        }
        else {
            this->backToMenu = true;
            break;
        }
    }
}

//��ȡ��ʷ��������
// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game_great::readLeaderBoard()
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

//��ȡ�������
bool Game_great::readNameBoard()
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
        if (fhand.eof()) break; // ��ֹ�ļ�ĩβ���һ��

        std::string temp(length, '\0');
        fhand.read(&temp[0], length);

        this->mNameBoard[i] = temp;
        i++;
    }

    fhand.close();
    return true;
}

//�������а�
bool Game_great::updateLeaderBoard()
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

//д����ʷ��������
bool Game_great::writeLeaderBoard()
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

//д���������
bool Game_great::writeNameBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mNameBoardFilePath, std::ios::binary | std::ios::trunc | std::ios::out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i++)
    {
        // д���ַ�������
        size_t length = this->mNameBoard[i].size();
        fhand.write(reinterpret_cast<const char*>(&length), sizeof(length));
        // д���ַ�������
        fhand.write(this->mNameBoard[i].c_str(), length);
    }
    fhand.close();
    return true;
}

//�����������
void Game_great::setName(string name)
{
    this->mName = name;
}

bool Game_great::getBack() const
{
    return this->backToMenu;
}
