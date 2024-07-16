#ifndef SNAKE_FUN_H
#define SNAKE_FUN_H

#include <vector>
#include <SFML/Audio.hpp>

enum class DirectionFun
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

class SnakeBodyFun
{
public:
    SnakeBodyFun();
    SnakeBodyFun(int x, int y);
    int getX() const;
    int getY() const;
    bool operator == (const SnakeBodyFun& snakeBody);
private:
    int mX;
    int mY;
};

// Snake class should have no depency on the GUI library
class SnakeFun
{
public:
    //Snake();
    SnakeFun(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    ~SnakeFun();
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Check if the snake is on the coordinate
    // bool isSnakeOn(int x, int y);
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    bool isPartOfFood(int x, int y);
    bool isPartOfObstacle(int x, int y); //changed by qjc
    void senseFood(SnakeBodyFun food, int score);
    void senseFood(std::vector<SnakeBodyFun> food, std::vector<int> score);
    // Check if hit wall
    bool hitWall();
    bool touchFood();
    int touchThing();
    int touchFoodIndex(int &x);


    void senseGate(SnakeBodyFun Ga1, SnakeBodyFun Ga2, SnakeBodyFun Ga3, SnakeBodyFun Ga4, SnakeBodyFun Ga5, SnakeBodyFun Ga6);
    void senseObstacle(std::vector<SnakeBodyFun> mObstacle); //changed by qjc
    void senseObstacle(SnakeBodyFun oneObstacle); //changed by qjc

    bool hitSelf();
    bool hitObstacle(); //changed by qjc
    bool checkCollision();
    bool changeDirection(DirectionFun newDirection);
    std::vector<SnakeBodyFun>& getSnake();
    int getLength();
    SnakeBodyFun createNewHead();
    SnakeBodyFun createNewHead(int x, int y);
    bool moveFoward();

    //changed by qjc
    //无敌状态
    void dicrCD() {if (invin_present_CD > 0) invin_present_CD--;};
    void dicrInvTime() {if (invin_present_time > 0) invin_present_time--;};
    int getCD() const {return invin_present_CD;};
    void setInvin();
    void resetInvin() {if (invin_present_time == 0) invincibility = false;}; //如果无敌时间结束，那就终止无敌状态
    bool checkInvinStatus() {return invincibility;};

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    DirectionFun mDirection;
    //SnakeBody mFood;

    std::vector<SnakeBodyFun> mFood;
    std::vector<int> mScore; //同步生成

    std::vector<SnakeBodyFun> mSnake;
    std::vector<SnakeBodyFun> Gate;
    std::vector<SnakeBodyFun> Obstacle; //changed by qjc

    //changed by qjc
    //无敌状态
    bool invincibility = false;
    const int invin_whole_CD = 100;
    const int invin_whole_time = 10;
    int invin_present_CD = 0;
    int invin_present_time = 0;

    sf::Sound* foodsound;
    sf::Sound* dingsound;
    sf::Sound* movesound;
};

#endif

