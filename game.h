#ifndef GAME_H
#define GAME_H

#include "curses.h"
#include <string>
#include <vector>
#include <memory>
#include <string>
#include <SFML/Audio.hpp>

#include "snake.h"

using namespace std;

class Game
{
public:
    Game();
    ~Game();

    void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;

    void createInstructionBoard();
    void renderInstructionBoard() const;

    void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool readNameBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    bool writeNameBoard();
    void renderLeaderBoard() const;

    void renderBoards() const;

    void initializeGame();
    void runGame();
    void renderPoints() const;
    void renderDifficulty() const;
    void renderCD() const;

    void createRamdonFood(int num1, int num2);
    void createRandomGate();
    void createRandomObstacle();
    void createRandomWater(int num);

    void renderFood() const;
    //void renderFood_first() const;
    //void renderFood_final() const;
    void renderSnake() const;
    void renderGate() const;
    void renderObstacle() const;
    void renderWater() const;
    bool isPartOfWater(int x, int y) const;
    int FindIndexWater(int x, int y) const;
    bool controlSnake() const;

    void startGame();
    int renderRestartMenu() const;
    int renderPauseMenu() const;
    void adjustDelay();

    void setName(string name);
    bool getBack() const;
    void dispScore();

private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    std::vector<SnakeBody> mFood;
    std::vector<int> mScore;
    std::vector<int> mCnt;
    int numOfWhiteFood = 3;
    int numOfBlueFood  = 2;

    std::vector<SnakeBody> Gate;
    std::vector<SnakeBody> mObstacle;
    const char mFoodSymbol_first = '#';
    const char mFoodSymbol_final = ' ';
    const char mGateSymbol = 'O';
    const char mObstacleSymbol = '%';
    const char mWaterSymbol = '*';
    int mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
    int numOfObstacles = 5;
    int mDelay;
    const std::string mRecordBoardFilePath = "point_record_fun.dat";
    const std::string mNameBoardFilePath = "name_record_fun.dat";
    std::vector<int> mLeaderBoard;
    std::vector<string> mNameBoard;
    const int mNumLeaders = 3;

    string mName;
    bool mAttempt = false;

    //蓝色食物
    std::vector<SnakeBody> water;
    float slippery = 1;
    //用来被delay除以，使得停留时间变短
    const int numOfWaters = 0;
    const int whole_slippery_time = 10;
    const int slipperRatio = 3;
    int present_slippery_time = 0;
    void dicrSlipperyTime() {if (present_slippery_time > 0) present_slippery_time--;}; //如果处于滑行阶段，那就让滑行时间减一
    void setSlippery() {present_slippery_time = whole_slippery_time; slippery = slipperRatio;}; //滑行的时候速度增加3倍
    void resetSlippery() {if (present_slippery_time == 0) slippery = 1;};

    sf::Sound* deadsound;
    sf::Sound* pausesound;
    sf::Sound* switchsound;

    bool backToMenu = false;
    bool exit = false;
};

#endif
