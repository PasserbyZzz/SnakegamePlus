#ifndef SNAKE_GREAT_H
#define SNAKE_GREAT_H

#include <vector>
#include <SFML/Audio.hpp>

enum class Direction_great
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

class SnakeBody_great
{
public:
    SnakeBody_great();
    SnakeBody_great(int x, int y);
    int getX() const;
    int getY() const;
    bool operator == (const SnakeBody_great& snakeBody);
private:
    int mX;
    int mY;
};

// Snake class should have no depency on the GUI library
class Snake_great
{
public:
    //Snake();
    Snake_great(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    ~Snake_great();
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
    void senseFood(SnakeBody_great food, int score);
    void senseFood(std::vector<SnakeBody_great> food, std::vector<int> score);
    // Check if hit wall
    bool hitWall();
    bool touchFood();
    int touchThing();
    int touchFoodIndex(int &x);


    void senseGate(SnakeBody_great Ga1, SnakeBody_great Ga2, SnakeBody_great Ga3, SnakeBody_great Ga4, SnakeBody_great Ga5, SnakeBody_great Ga6);
    void senseObstacle(std::vector<SnakeBody_great> mObstacle); //changed by qjc
    void senseObstacle(SnakeBody_great oneObstacle); //changed by qjc

    bool hitSelf();
    bool hitObstacle(); //changed by qjc
    bool checkCollision();
    bool changeDirection(Direction_great newDirection);
    std::vector<SnakeBody_great>& getSnake();
    int getLength();
    SnakeBody_great createNewHead();
    SnakeBody_great createNewHead(int x, int y);
    bool moveFoward();

    //changed by qjc
    //�޵�״̬
    void dicrCD() {if (invin_present_CD > 0) invin_present_CD--;};
    void dicrInvTime() {if (invin_present_time > 0) invin_present_time--;};
    int getCD() const {return invin_present_CD;};
    void setInvin();
    void resetInvin() {if (invin_present_time == 0) invincibility = false;}; //����޵�ʱ��������Ǿ���ֹ�޵�״̬
    bool checkInvinStatus() {return invincibility;};

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    Direction_great mDirection;
    //SnakeBody_great mFood;

    std::vector<SnakeBody_great> mFood;
    std::vector<int> mScore; //ͬ������

    std::vector<SnakeBody_great> mSnake;
    std::vector<SnakeBody_great> Gate;
    std::vector<SnakeBody_great> Obstacle; //changed by qjc

    //changed by qjc
    //�޵�״̬
    bool invincibility = false;
    const int invin_whole_CD = 0;
    const int invin_whole_time = 10;
    int invin_present_CD = 0;
    int invin_present_time = 0;

    sf::Sound* foodsound;
    sf::Sound* travelsound;
    sf::Sound* movesound;
    sf::Sound* dingsound;
};

#endif
