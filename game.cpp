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

//���캯��
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

//��������
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

//���������
void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

//��ʾ���
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

//������Ϸ��
void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

//��ʾ��Ϸ����
void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

//������ʾ��
void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

//��ʾ��ʾ
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

//��ʾ���а�
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

//��Ϸ���������û�ѡ��Restart����Quit
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

//��Ϸ��ͣ�����û�ѡ��Continue����Quit
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

//�޸ķ���������ʾ����ʾ��
void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

//�޸��Ѷȣ�����ʾ����ʾ��
void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

//�޸�CDʱ��������ʾ����ʾ��
void Game::renderCD() const
{
    int CD = this->mPtrSnake->getCD();
    std::string CDString;
    if (CD > 10) CDString = std::to_string(CD / 10 * 10); //����̫���ʱ��Ͳ�����ʾ��λ���ˣ���Ȼ�����۾���
    else {
        CDString = "0"+ std::to_string(CD);
        CDString = CDString.substr(0,2); //ȥ��ĩβ��0
    }
    mvwprintw(this->mWindows[2], 15, 1, CDString.c_str());
    wrefresh(this->mWindows[2]);
}

//��ʼ����Ϸ����
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
    this->mObstacle.clear(); //��ÿһ����Ϸ��ʼ֮ǰ�����һ����Ϸ�������ϰ���
    this->createRandomObstacle(); //�˴��ں����ڲ��������sense��������û������һ��sense
    this->water.clear();
    this->createRandomWater(this->numOfWaters);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mDelay = this->mBaseDelay;
}

//�������ʳ��
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
            continue; //������ߵ�һ���ֻ���ʳ���һ���ֻ����Ѿ���������ϰ���Ǿ���Ҫ���������ϰ���
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
        //�ж�ʳ���Ƿ�Ӧ����ʾ
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

        //�ж���ɫ
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

//��ʾ��
void Game::renderSnake() const {
    int snakeLength = this->mPtrSnake->getLength();
    const std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();  // �޸�Ϊconst���ã����ⲻ��Ҫ�ĸ���

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

//ˢ�´��� ������������
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

//�ı���Ϸ�Ѷȣ�ÿ��5���Ѷ�+1��������5������ʱ����Ϸ����
void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

//������Ϸ
void Game::runGame()
{
    action:
    int suspend = 0;

    while (true)
    {
        SnakeBody head = this->mPtrSnake->getSnake()[0];
        if (isPartOfWater(head.getX(), head.getY())) {
            int index = FindIndexWater(head.getX(), head.getY());
            water.erase(water.begin()+index); //ɾ������ɫʳ�ﲢ������µ���ɫʳ��
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
        //Ϊ��ģ���ٶȱ�����׷ʳ������
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

        //ʳ����ʧ
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

//��ʼ��Ϸ
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

//��ȡ��ʷ��������
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

//��ȡ�������
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

//д����ʷ��������
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

//д���������
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
void Game::setName(string name)
{
    this->mName = name;
}

bool Game::getBack() const
{
    return this->backToMenu;
}
