#ifndef GAMEPAIR_H
#define GAMEPAIR_H

#include "curses.h"
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <SFML/Audio.hpp>

#include "snake_pair.h"

using namespace std;

class GamePair
{
public:
    GamePair();
    ~GamePair();

    void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;

    void createInstructionBoard();
    void renderInstructionBoard() const;

    void renderBoards() const;

    void initializeGame();
    int runGame();
    void renderPoints() const;
    void renderTime();

    void createRamdomFoodall();
    void createRamdomFoodone();
    void createRandomGate();
    void createRandomObstacle();

    void renderFood() const;
    void renderGate() const;
    void renderObstacle() const;
    void renderSnake() const;

    bool controlSnake1();
    bool controlSnake2();

    void startGame();
    int renderRestartMenu(int x) const;
    int renderPauseMenu() const;

    void setName(string name1, string name2);
    bool getBack() const;

    void getInitTime();
    void getpause_time1();
    void getpause_time2();
    int countDown();

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
    std::unique_ptr<SnakePair> mPtrSnake1;
    std::unique_ptr<SnakePair> mPtrSnake2;
    // Food information
    vector<SnakeBodyPair> mFood;
    std::vector<SnakeBodyPair> Gate;
    std::vector<SnakeBodyPair> mObstacle;

    const char mFoodSymbol = '#';
    const char mGateSymbol = 'O';
    const char mObstacleSymbol = '%';
    const char mSnakeSymbol = '@';

    int mPoints[2] = {0,0};
    int numOfObstacles = 5;
    int mBaseDelay = 100;
    int mDelay;
    //const std::string mRecordBoardFilePath = "point_record.dat";
    //const std::string mNameBoardFilePath = "name_record.dat";
    //std::vector<int> mLeaderBoard;
    //std::vector<string> mNameBoard;
    //const int mNumLeaders = 3;

    string mName1;
    string mName2;
    bool backToMenu = false;

    time_t init_time;
    time_t pause_time1;
    int pause_time2;

    sf::Sound* movesound;
    sf::Sound* deadsound;
    sf::Sound* pausesound;
    sf::Sound* switchsound;
};

#endif
