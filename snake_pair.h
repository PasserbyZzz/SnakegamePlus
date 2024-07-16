#ifndef SNAKE_PAIR_H
#define SNAKE_PAIR_H

#include <vector>

static int num = 1;

enum class DirectionPair
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

class SnakeBodyPair
{
    public:
        SnakeBodyPair();
        SnakeBodyPair(int x, int y);
        int getX() const;
        int getY() const;
        bool operator == (const SnakeBodyPair& snakeBody);
    private:
        int mX;
        int mY;
};


class SnakePair
{
public:
    //Snake();
    SnakePair(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    bool isPartOfFood(int x, int y);
    bool isPartOfObstacle(int x, int y);

    void senseFood(SnakeBodyPair food);
    void senseGate(SnakeBodyPair Ga1, SnakeBodyPair Ga2, SnakeBodyPair Ga3, SnakeBodyPair Ga4, SnakeBodyPair Ga5, SnakeBodyPair Ga6);
    void senseObstacle(std::vector<SnakeBodyPair> mObstacle);
    void senseObstacle(SnakeBodyPair oneObstacle);

    int touchThing(int &x);
    void eraseFood(int x);
    // Check if the snake is dead
    bool hitWall();
    bool hitSelf();
    bool hitObstacle();
    bool hitOther(SnakePair other);
    bool checkCollision(SnakePair other);

    bool changeDirection(DirectionPair newDirection);
    std::vector<SnakeBodyPair>& getSnake();
    int getLength();
    SnakeBodyPair createNewHead();
    bool moveFoward(int &x);
    DirectionPair getDirection();

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    DirectionPair mDirection;
    std::vector<SnakeBodyPair> mFood;
    std::vector<SnakeBodyPair> mSnake;
    std::vector<SnakeBodyPair> Gate;
    std::vector<SnakeBodyPair> Obstacle;
};

#endif

