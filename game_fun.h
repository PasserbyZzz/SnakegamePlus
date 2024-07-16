#ifndef GAME_FUN_H
#define GAME_FUN_H

#include "curses.h"
#include <string>
#include <vector>
#include <memory>
#include <string>
#include <SFML/Audio.hpp>

#include "snake_fun.h"

using namespace std;

class GameFun
{
public:
    GameFun();
    ~GameFun();

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
    void renderCD() const; //changed by qjc

    int generateRandomNumber(); //����������ɲ�ͬ��ֵ��ʳ��
    void createRamdonFood();
    void createRandomGate();
    void createRandomObstacle(); //changed by qjc
    void createRandomWater(int num); //changed by qjc

    void renderFood_first() const;
    void renderFood_final() const;
    void renderSnake() const;
    void renderGate() const;
    void renderObstacle() const; //changed by qjc
    void renderWater() const; //changed by qjc
    bool isPartOfWater(int x, int y) const;
    int FindIndexWater(int x, int y) const;  //changed by qjc
    bool controlSnake();

    void startGame();
    int renderRestartMenu() const;
    int renderPauseMenu() const;
    void adjustDelay();

    void setName(string name);
    bool getBack() const;


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

    std::unique_ptr<SnakeFun> mPtrSnake;
    // Food information
    std::vector<SnakeBodyFun> mFood;
    std::vector<int> mScore;
    std::vector<SnakeBodyFun> Gate;
    std::vector<SnakeBodyFun> mObstacle; //changed by qjc

    const char mSnakeSymbol = '@';
    const char mFoodSymbol_first = '#';
    const char mFoodSymbol_final = ' ';
    const char mGateSymbol = 'O';
    const char mObstacleSymbol = '%'; //changed by qjc
    const char mWaterSymbol = '*'; //changed by qjc

    int mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
    const int mRecreationDelay = 100;
    int numOfObstacles = 5; //changed by qjc
    int mDelay;
    const std::string mRecordBoardFilePath = "point_record.dat";
    const std::string mNameBoardFilePath = "name_record.dat";
    std::vector<int> mLeaderBoard;
    std::vector<string> mNameBoard;
    const int mNumLeaders = 3;

    string mName;
    int mCnt = 0;
    bool mAttempt = false;

    //��ˮ��ò
    //changed by qjc
    std::vector<SnakeBodyFun> water;
    float slippery = 1; //������delay���ԣ�ʹ��ͣ��ʱ����
    const int numOfWaters = 0;
    const int whole_slippery_time = 10;
    const int slipperRatio = 3;
    int present_slippery_time = 0;
    void dicrSlipperyTime() {if (present_slippery_time > 0) present_slippery_time--;}; //������ڻ��н׶Σ��Ǿ��û���ʱ���һ
    void setSlippery() {present_slippery_time = whole_slippery_time; slippery *= slipperRatio;}; //���е�ʱ���ٶ�����3��
    void resetSlippery() {if (present_slippery_time == 0) slippery = 1;};

    //���ʱ��
    const int maxMove = 500;

    sf::Sound* movesound;
    sf::Sound* deadsound;
    sf::Sound* pausesound;
    sf::Sound* switchsound;

    bool backToMenu = false;
    bool exit = false;
};



#endif

