#include <string>
#include <iostream>
#include <cmath>
#include <windows.h>
#include <stdlib.h>

// For terminal delay
#include <chrono>
#include <thread>
#include <future>
#include <ctime>

#include <fstream>
#include <algorithm>
#include <SFML/Audio.hpp>

#include "curses.h"
#include "color.h"
#include "game_pair.h"

using namespace std;

GamePair::GamePair()
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

    initColors();

    this->pausesound = gameSound("game_start.wav");
    this->deadsound = gameSound("game_over.wav");
    this->switchsound = gameSound("light_switch.wav");
}

GamePair::~GamePair()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        int a = delwin(this->mWindows[i]);
    }
    endwin();

    delete deadsound;
    delete pausesound;
    delete switchsound;
}

void GamePair::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void GamePair::renderInformationBoard() const
{
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/PasserbyZzz/SnakegamePlus");
    mvwprintw(this->mWindows[0], 4, 1, "Team Members: DZX, QJC and XKY");
    wrefresh(this->mWindows[0]);
}

void GamePair::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void GamePair::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void GamePair::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void GamePair::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");
    mvwprintw(this->mWindows[2], 2, 1, "Snakel   Snake2");
    mvwprintw(this->mWindows[2], 3, 1, "  W        UP  ");
    mvwprintw(this->mWindows[2], 4, 1, "  S       DOWN ");
    mvwprintw(this->mWindows[2], 5, 1, "  A       LEFT ");
    mvwprintw(this->mWindows[2], 6, 1, "  D       RIGHT");
    mvwprintw(this->mWindows[2], 8, 1, "CountingDown:");
    mvwprintw(this->mWindows[2], 11, 1, "Points");

    wrefresh(this->mWindows[2]);
}

int GamePair::renderRestartMenu(int k) const
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
    mvwprintw(menu, 1, 1, "The Final Winner:");
    string namex;

    if (k == 0)
    {
        if (mPoints[0] > mPoints[1])
            namex = this->mName1;
        else if (mPoints[0] < mPoints[1])
            namex = this->mName2;
        else
            namex = "A Draw!";
    }
    if (k == 1)
        namex = this->mName2;
    else if (k == 2)
        namex = this->mName1;

    mvwprintw(menu, 2, 1, namex.c_str());
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

int GamePair::renderPauseMenu() const
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

void GamePair::renderPoints() const
{
    std::string pointString1 = std::to_string(this->mPoints[0]);
    std::string pointString2 = std::to_string(this->mPoints[1]);

    if (mPoints[0] >= mPoints[1])
    {
        mvwprintw(this->mWindows[2], 12, 1, this->mName1.c_str());
        mvwprintw(this->mWindows[2], 13, 1, this->mName2.c_str());
        mvwprintw(this->mWindows[2], 12, 7, pointString1.c_str());
        mvwprintw(this->mWindows[2], 13, 7, pointString2.c_str());
    }
    else
    {
        mvwprintw(this->mWindows[2], 12, 1, this->mName2.c_str());
        mvwprintw(this->mWindows[2], 13, 1, this->mName1.c_str());
        mvwprintw(this->mWindows[2], 12, 7, pointString2.c_str());
        mvwprintw(this->mWindows[2], 13, 7, pointString1.c_str());
    };

    wrefresh(this->mWindows[2]);
}

void GamePair::renderTime()
{
    int x = this->countDown();
    string timex = to_string(x);
    mvwprintw(this->mWindows[2],9,1,timex.c_str());
}

void GamePair::initializeGame()
{
    // allocate memory for a new snake
    this->mPtrSnake1.reset(new SnakePair(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->mPtrSnake2.reset(new SnakePair(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));

    this->mFood.clear();
    this->createRamdomFoodall();

    this->mObstacle.clear();
    this->createRandomObstacle();

    this->createRandomGate();
    this->createRandomGate();
    this->createRandomGate();
    this->mPtrSnake1->senseGate(this->Gate[0], this->Gate[1], this->Gate[2], this->Gate[3], this->Gate[4], this->Gate[5]);
    this->mPtrSnake2->senseGate(this->Gate[0], this->Gate[1], this->Gate[2], this->Gate[3], this->Gate[4], this->Gate[5]);

    this->mPoints[0] = 0;
    this->mPoints[1] = 0;
    this->pause_time2 = 0;
    this->pause_time1 = 0;

    this->mDelay = this->mBaseDelay;
}

void GamePair::createRamdomFoodall()
{
    for (int i = 0; i < 15; i++)
    {
        int x = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y = (rand() % (this->mGameBoardHeight - 2)) + 1;
        while (this->mPtrSnake1->isPartOfSnake(x,y) || this->mPtrSnake2->isPartOfSnake(x,y))
        {
            x = (rand() % (this->mGameBoardWidth - 2)) + 1;
            y = (rand() % (this->mGameBoardHeight - 2)) + 1;
        };
        SnakeBodyPair food = SnakeBodyPair(x,y);
        this->mFood.push_back(food);
        this->mPtrSnake1->senseFood(food);
        this->mPtrSnake2->senseFood(food);
    }
}

void GamePair::createRamdomFoodone()
{
    int x = (rand() % (this->mGameBoardWidth - 2)) + 1;
    int y = (rand() % (this->mGameBoardHeight - 2)) + 1;
    while (this->mPtrSnake1->isPartOfSnake(x,y) || this->mPtrSnake2->isPartOfSnake(x,y) || this->mPtrSnake1->isPartOfFood(x,y) || this->mPtrSnake1->isPartOfObstacle(x,y))
    {
        x = (rand() % (this->mGameBoardWidth - 2)) + 1;
        y = (rand() % (this->mGameBoardHeight - 2)) + 1;
    };
    SnakeBodyPair food = SnakeBodyPair(x,y);
    this->mFood.push_back(food);
    this->mPtrSnake1->senseFood(food);
    this->mPtrSnake2->senseFood(food);
}

void GamePair::renderFood() const
{
    for (int i = 0; i < 15; i++)
    {
        mvwaddch(this->mWindows[1], this->mFood[i].getY(), this->mFood[i].getX(), this->mFoodSymbol);
    };
    wrefresh(this->mWindows[1]);
}

void GamePair::createRandomGate()
{
    int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
    int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;
    int x2 = (rand() % (this->mGameBoardWidth - 2)) + 1;
    int y2 = (rand() % (this->mGameBoardHeight - 2)) + 1;
    while (this->mPtrSnake1->isPartOfSnake(x1,y1) || this->mPtrSnake1->isPartOfSnake(x2,y2) || this->mPtrSnake2->isPartOfSnake(x1,y1) || this->mPtrSnake2->isPartOfSnake(x2,y2)|| this->mPtrSnake1->isPartOfFood(x2,y2))
    {
        x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;
        x2 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        y2 = (rand() % (this->mGameBoardHeight - 2)) + 1;
    }
    SnakeBodyPair Gate1 = SnakeBodyPair(x1,y1);
    SnakeBodyPair Gate2 = SnakeBodyPair(x2,y2);
    this->Gate.push_back(Gate1);
    this->Gate.push_back(Gate2);
}

void GamePair::renderGate() const
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

void GamePair::createRandomObstacle()
{
    int currentObstaclenum = 0;
    while (currentObstaclenum < this->numOfObstacles) {
        int x1 = (rand() % (this->mGameBoardWidth - 2)) + 1;
        int y1 = (rand() % (this->mGameBoardHeight - 2)) + 1;
        if (this->mPtrSnake1->isPartOfSnake(x1,y1) || this->mPtrSnake2->isPartOfSnake(x1,y1) || this->mPtrSnake1->isPartOfFood(x1,y1) || this->mPtrSnake1->isPartOfObstacle(x1,y1)) {
            continue;
        }
        SnakeBodyPair oneObstacle = SnakeBodyPair(x1, y1);
        this->mObstacle.push_back(oneObstacle);
        this->mPtrSnake1->senseObstacle(oneObstacle);
        this->mPtrSnake2->senseObstacle(oneObstacle);
        currentObstaclenum++;
    }
}

void GamePair::renderObstacle() const
{
    wattron(this->mWindows[1], COLOR_PAIR(6));
    for (SnakeBodyPair oneObstacle: this->mObstacle) {
        mvwaddch(this->mWindows[1], oneObstacle.getY(), oneObstacle.getX(), this->mObstacleSymbol);
    }
    wattroff(this->mWindows[1], COLOR_PAIR(6));
    wrefresh(this->mWindows[1]);
}

void GamePair::renderSnake() const
{
    int snake1Length = this->mPtrSnake1->getLength();
    int snake2Length = this->mPtrSnake2->getLength();
    std::vector<SnakeBodyPair>& snake_1 = this->mPtrSnake1->getSnake();
    std::vector<SnakeBodyPair>& snake_2 = this->mPtrSnake2->getSnake();
    for (int i = 0; i < snake1Length; i ++)
    {
        if (i % 2 == 0) {
            wattron(this->mWindows[1], COLOR_PAIR(4));  // 启用绿色
        } else {
            wattron(this->mWindows[1], COLOR_PAIR(1));  // 启用白色
        }

        mvwaddch(this->mWindows[1], snake_1[i].getY(), snake_1[i].getX(), this->mSnakeSymbol);

        if (i % 2 == 0) {
            wattroff(this->mWindows[1], COLOR_PAIR(4));  // 关闭绿色
        } else {
            wattroff(this->mWindows[1], COLOR_PAIR(1));  // 关闭白色
        }
    }

    for (int i = 0; i < snake2Length; i ++)
    {
       if (i % 2 == 0) {
            wattron(this->mWindows[1], COLOR_PAIR(3));  // 启用绿色
        } else {
            wattron(this->mWindows[1], COLOR_PAIR(1));  // 启用白色
        }

        mvwaddch(this->mWindows[1], snake_2[i].getY(), snake_2[i].getX(), this->mSnakeSymbol);

        if (i % 2 == 0) {
            wattroff(this->mWindows[1], COLOR_PAIR(3));  // 关闭绿色
        } else {
            wattroff(this->mWindows[1], COLOR_PAIR(1));  // 关闭白色
        }
    }

    wrefresh(this->mWindows[1]);
}

bool GamePair::controlSnake1()
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        {
            this->mPtrSnake1->changeDirection(DirectionPair::Up);
            break;
        }
        case 'S':
        case 's':
        {
            this->mPtrSnake1->changeDirection(DirectionPair::Down);
            break;
        }
        case 'A':
        case 'a':
        {
            this->mPtrSnake1->changeDirection(DirectionPair::Left);
            break;
        }
        case 'D':
        case 'd':
        {
            this->mPtrSnake1->changeDirection(DirectionPair::Right);
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

bool GamePair::controlSnake2()
{
    short k1,k2,k3,k4,k5;
    k1 = GetAsyncKeyState(VK_UP);
    k2 = GetAsyncKeyState(VK_DOWN);
    k3 = GetAsyncKeyState(VK_LEFT);
    k4 = GetAsyncKeyState(VK_RIGHT);
    k5 = GetAsyncKeyState(VK_SPACE);

    if (k1 & 0x8000)
    {
        this->mPtrSnake2->changeDirection(DirectionPair::Up);
        return true;
    };

    if (k2 & 0x8000)
    {
        this->mPtrSnake2->changeDirection(DirectionPair::Down);
        return true;
    };

    if (k3 & 0x8000)
    {
        this->mPtrSnake2->changeDirection(DirectionPair::Left);
        return true;
    };

    if (k4 & 0x8000)
    {
        this->mPtrSnake2->changeDirection(DirectionPair::Right);
        return true;
    };

    return true;
}

void GamePair::renderBoards() const
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
}

int GamePair::runGame()
{
    int key = 0;

    action:

    int suspend = 0;
    int timex = this->countDown();

    while (timex > 0)
    {
        timex = this->countDown();

        int a,b;
        future<bool> s1 = async(&GamePair::controlSnake1,this);
        future<bool> s2 = async(&GamePair::controlSnake2,this);
        bool x,y;
        x = s1.get();
        y = s2.get();

        if (!x || !y)
        {
            suspend = 1;
            this->getpause_time1();
            break;
        }

        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);

        bool eatFood1 = this->mPtrSnake1->moveFoward(a);
        bool collision1 = this->mPtrSnake1->checkCollision(*this->mPtrSnake2);
        if (eatFood1 == true)
        {
            this->mFood.erase(this->mFood.begin()+a);
            this->mPtrSnake2->eraseFood(a);
            this->mPoints[0] += 1;
            this->createRamdomFoodone();
        };

        bool eatFood2 = this->mPtrSnake2->moveFoward(b);
        bool collision2 = this->mPtrSnake2->checkCollision(*this->mPtrSnake1);
        if (eatFood2 == true)
        {
            this->mFood.erase(this->mFood.begin()+b);
            this->mPtrSnake1->eraseFood(b);
            this->mPoints[1] += 1;
            this->createRamdomFoodone();
        };

        if (collision1 == true && collision2 == true)
        {
            this->deadsound->play();
            key = 0;
            break;
        }

        if (collision1 == true)
        {
            this->deadsound->play();
            key = 1;
            break;
        };
        if (collision2 == true)
        {
            this->deadsound->play();
            key = 2;
            break;
        }

        this->renderObstacle();
        this->renderGate();
        this->renderSnake();
        this->renderPoints();
        this->renderTime();
        this->renderFood();

        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }

    if (suspend == 1)
    {
        while (true)
        {
            this->renderSnake();
            this->renderFood();
            this->renderPoints();
            int y = this->renderPauseMenu();
            if (y == 0)
            {
                this->getpause_time2();
                goto action;
            }
            else if (y == 2)
                exit(0);
            else {
                this->backToMenu = true;
                break;
            }
        }
    }

    return key;
}

void GamePair::startGame()
{
    refresh();
    int choice;
    while (true)
    {
        this->renderBoards();
        this->initializeGame();
        this->getInitTime();
        int x = this->runGame();

        if (this->backToMenu == true)
            break;

        choice = this->renderRestartMenu(x);
        if (choice == 2)
            exit(0);
        else if (choice == 0) {
            continue;
        }
        else {
            this->backToMenu = true;
            break;
        }
    }
}


void GamePair::setName(string name1, string name2)
{
    this->mName1 = name1;
    this->mName2 = name2;
}

bool GamePair::getBack() const
{
    return this->backToMenu;
}

void GamePair::getInitTime()
{
    time(&this->init_time);
}

void GamePair::getpause_time1()
{
    time_t pausetime1;
    time(&pausetime1);
    this->pause_time1 = pausetime1;
}

void GamePair::getpause_time2()
{
    time_t pausetime2;
    time(&pausetime2);
    int difftime = pausetime2 - this->pause_time1;
    this->pause_time2 += difftime;
}

int GamePair::countDown()
{
    time_t time_now;
    time(&time_now);

    int difftime = time_now - this->init_time;
    int duration = difftime - this->pause_time2;

    return 180 - difftime;
}


