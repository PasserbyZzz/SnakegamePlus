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

    void createRamdonFood();
    void createRandomGate();
    void renderFood_first() const;
    void renderFood_final() const;
    void renderSnake() const;
    void renderGate() const;
    bool controlSnake() const;

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
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    std::vector<SnakeBody> Gate;
    const char mFoodSymbol_first = '#';
    const char mFoodSymbol_final = ' ';
    const char mGateSymbol = 'O';
    int mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
    int mDelay;
    const std::string mRecordBoardFilePath = "point_record.dat";
    const std::string mNameBoardFilePath = "name_record.dat";
    std::vector<int> mLeaderBoard;
    std::vector<string> mNameBoard;
    const int mNumLeaders = 3;

    string mName;
    int mCnt = 0;
    bool mAttempt = false;
    sf::Sound* movesound;
    sf::Sound* deadsound;
    sf::Sound* pausesound;
    sf::Sound* switchsound;
    bool backToMenu = false;
};

#endif
