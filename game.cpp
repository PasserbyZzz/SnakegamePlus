#include <string>
#include <iostream>
#include <cmath>
#include <string>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>

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

    initColors();
}

//析构函数
Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
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
    string name =  this->mName + "!";

    if (!this->mAttempt)
        mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game,");
    else
        mvwprintw(this->mWindows[0], 1, 1, "Welcome Back! Good Luck This Time,");

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

    wrefresh(this->mWindows[2]);
}

//显示排行榜
void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 14 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 14, 1, "Leader Board");
    std::string pointnameString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 14 - 2); i ++)
    {
        pointnameString = std::to_string(this->mLeaderBoard[i]) + " --" + this->mNameBoard[i];
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 14 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 14 + (i + 1), 5, pointnameString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

//游戏结束后，让用户选择Restart还是Quit
bool Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

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
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

//游戏暂停后，让用户选择Continue还是Quit
bool Game::renderPauseMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Continue", "Quit"};

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
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

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

//初始化游戏界面
void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->createRamdonFood();
    this->mPtrSnake->senseFood(this->mFood);
    this->createRandomGate();
    this->createRandomGate();
    this->createRandomGate();
    this->mPtrSnake->senseGate(this->Gate[0], this->Gate[1], this->Gate[2], this->Gate[3], this->Gate[4], this->Gate[5]);
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mCnt = 0;
    this->mDelay = this->mBaseDelay;
}

//随机生成食物
void Game::createRamdonFood()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            if(this->mPtrSnake->isPartOfSnake(j, i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mFood = availableGrids[random_idx];
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

//显示食物
void Game::renderFood_first() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol_first);
    wrefresh(this->mWindows[1]);
}

//食物即将消失
void Game::renderFood_final() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol_final);
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

//显示蛇
void Game::renderSnake() const {
    int snakeLength = this->mPtrSnake->getLength();
    const std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();  // 修改为const引用，避免不必要的复制

    for (int i = 0; i < snakeLength; i++) {
        if (i % 2 == 0) {
            wattron(this->mWindows[1], COLOR_PAIR(2));  // 启用绿色
        } else {
            wattron(this->mWindows[1], COLOR_PAIR(1));  // 启用白色
        }

        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);

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
        case ' ':
        {
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
        bool x = this->controlSnake();
        if (!x)
        {
            suspend = 1;
            break;
        }
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);

        bool eatFood = this->mPtrSnake->moveFoward();
        bool collision = this->mPtrSnake->checkCollision();
        if (collision == true)
        {
            break;
        }
        this->renderGate();
        this->renderSnake();
        if (eatFood == true)
        {
            this->mPoints += 1;
            this->mCnt = 0;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            this->adjustDelay();
        }

        //食物很安全
        if (this->mCnt <= 0.75 * (80 / sqrt(this->mDifficulty + 1))) {
            this->renderFood_first();
            this->renderDifficulty();
            this->renderPoints();
        }

        //食物即将消失
        if (this->mCnt > 0.75 * (80 / sqrt(this->mDifficulty + 1)) && this->mCnt <= (80 / sqrt(this->mDifficulty + 1))) {
            if (this->mCnt % 2) {
                this->renderFood_first();
                this->renderDifficulty();
                this->renderPoints();
            }
            else {
                this->renderFood_final();
                this->renderDifficulty();
                this->renderPoints();
            }
        }

        this->mCnt ++;
        //食物消失
        if (this->mCnt > (80 / sqrt(this->mDifficulty + 1)) ) {
            this->createRamdonFood();
            this->mCnt = 0;
            this->mPtrSnake->senseFood(this->mFood);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

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
            bool y = this->renderPauseMenu();
            if(y)
                goto action;
            else
                break;
        }
    }
}

//开始游戏
void Game::startGame()
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
        choice = this->renderRestartMenu();
        if (choice == false)
            break;
        else
            this->mAttempt = true;
            this->Gate.clear();
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
