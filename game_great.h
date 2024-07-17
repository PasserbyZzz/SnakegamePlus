#ifndef GAME_GREAT_H
#define GAME_GREAT_H

#include "curses.h"
#include <string>
#include <vector>
#include <memory>
#include <string>
#include <SFML/Audio.hpp>

#include "snake_great.h"

using namespace std;

class Game_great
{
public:
    Game_great();
    ~Game_great();

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
    void createRamdonFood(int num); //����ָ��������ʳ��
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
    bool adjustDelay();

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

    std::unique_ptr<Snake_great> mPtrSnake;
    // Food information
    std::vector<SnakeBody_great> mFood;
    std::vector<int> mScore;
    std::vector<SnakeBody_great> Gate;
    std::vector<SnakeBody_great> mObstacle; //changed by qjc

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
    int mDelay = 100;
    const std::string mRecordBoardFilePath = "point_record_great.dat";
    const std::string mNameBoardFilePath = "name_record_great.dat";
    std::vector<int> mLeaderBoard;
    std::vector<string> mNameBoard;
    const int mNumLeaders = 3;

    string mName;
    int mCnt = 0;
    bool mAttempt = false;

    //��ˮ��ò
    //changed by qjc
    std::vector<SnakeBody_great> water;
    float slippery = 1; //������delay���ԣ�ʹ��ͣ��ʱ����
    const int numOfWaters = 0;
    const int whole_slippery_time = 8;
    const int slipperRatio = 3;
    int present_slippery_time = 0;
    void dicrSlipperyTime() {if (present_slippery_time > 0) present_slippery_time--;}; //������ڻ��н׶Σ��Ǿ��û���ʱ���һ
    void setSlippery() {present_slippery_time = whole_slippery_time; slippery *= slipperRatio;}; //���е�ʱ���ٶ�����3��
    void resetSlippery() {if (present_slippery_time == 0) slippery = 1;};

    //���ʱ��
    const int maxMove = 500;
    const int oneStageTime = 200;
    bool gainorlose = true;
    int gainPointsInOneStage = 0;

    bool backToMenu = false;
    bool exit = false;

    sf::Sound* deadsound;
    sf::Sound* pausesound;
    sf::Sound* switchsound;
    sf::Sound* dingsound;
};



#endif

