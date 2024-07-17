#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <SFML/Audio.hpp>

enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    int getX() const;
    int getY() const;
    bool operator == (const SnakeBody& snakeBody);
private:
    int mX;
    int mY;
};

// Snake class should have no depency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    ~Snake();
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Check if the snake is on the coordinate
    // bool isSnakeOn(int x, int y);
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    bool isPartOfFood(int x, int y);
    bool isPartOfObstacle(int x, int y);
    void senseFood(SnakeBody food, int score);
    void senseFood(std::vector<SnakeBody> food, std::vector<int> score);
    // Check if hit wall
    bool hitWall();
    bool touchFood();
    int touchThing();
    int touchFoodIndex(int &x);
    void senseGate(SnakeBody Ga1, SnakeBody Ga2, SnakeBody Ga3, SnakeBody Ga4, SnakeBody Ga5, SnakeBody Ga6);
    void senseObstacle(std::vector<SnakeBody> mObstacle);
    void senseObstacle(SnakeBody oneObstacle);
    bool hitSelf();
    bool hitObstacle();
    bool checkCollision();
    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    SnakeBody createNewHead();
    bool moveFoward();

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
    Direction mDirection;
    std::vector<SnakeBody> mFood;
    std::vector<int> mScore;
    std::vector<SnakeBody> mSnake;
    std::vector<SnakeBody> Gate;
    std::vector<SnakeBody> Obstacle;
    sf::Sound* foodsound;
    sf::Sound* travelsound;
    sf::Sound* movesound;
    sf::Sound* dingsound;

    //无敌状态
    bool invincibility = false;
    const int invin_whole_CD = 100;
    const int invin_whole_time = 10;
    int invin_present_CD = 0;
    int invin_present_time = 0;
};

#endif
