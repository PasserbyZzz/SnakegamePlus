#ifndef GAME_H
#define GAME_H

#include "curses.h"
#include <string>
#include <vector>
#include <memory>
#include <string>

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
    void renderFood() const;
    void renderSnake() const;
    void controlSnake() const;

		void startGame();
    bool renderRestartMenu() const;
    void adjustDelay();

    void setName(string name);


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
    const char mFoodSymbol = '#';
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
};

#endif
