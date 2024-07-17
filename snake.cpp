#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake.h"
#include "color.h"

SnakeBody::SnakeBody()
{
}


SnakeBody::SnakeBody(int x, int y): mX(x), mY(y)
{
}

int SnakeBody::getX() const
{
    return mX;
}

int SnakeBody::getY() const
{
    return mY;
}

bool SnakeBody::operator == (const SnakeBody& snakeBody)
{
    return (this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY());
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
    this->travelsound = gameSound("travel_through_wormhole.wav");
    this->foodsound = gameSound("game_bonus.wav");
    this->movesound = gameSound("snake_move.wav");
    this->dingsound = gameSound("ding.wav");
}

Snake::~Snake()
{
    delete travelsound;
    delete foodsound;
    delete movesound;
    delete dingsound;
}

void Snake::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void Snake::initializeSnake()
{
    // Instead of using a random initialization algorithm
    // We always put the snake at the center of the game mWindows
    int w = this->mGameBoardWidth / 5;
    int h = this->mGameBoardHeight / 5;
    int W = 3 * w;
    int H = 3 * h;

    int centerX = w + rand() % W;
    int centerY = h + rand() % H;

    switch (rand() % 4) {
        case 0:
            this->mDirection = Direction::Up;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBody(centerX, centerY + i));
            }
            break;
        case 1:
            this->mDirection = Direction::Down;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBody(centerX, centerY - i));
            }
            break;
        case 2:
            this->mDirection = Direction::Left;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBody(centerX + 1, centerY));
            }
            break;
        case 3:
            this->mDirection = Direction::Right;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBody(centerX - 1, centerY));
            }
            break;
    }
}

bool Snake::isPartOfSnake(int x, int y)
{
    SnakeBody temp = SnakeBody(x, y);
    for (int i = 0; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == temp)
        {
            return true;
        }
    }
    return false;
}

bool Snake::isPartOfObstacle(int x, int y)
{
    SnakeBody temp = SnakeBody(x, y);
    for (int i = 0; i < this->Obstacle.size(); i ++)
    {
        if (this->Obstacle[i] == temp)
        {
            return true;
        }
    }
    return false;
}

bool Snake::isPartOfFood(int x, int y)
{
    SnakeBody temp = SnakeBody(x, y);
    for (SnakeBody oneFood: this->mFood) {
        if (oneFood == temp)
            return true;
    }
    return false;
}

/*
 * Assumption:
 * Only the head would hit wall.
 */
bool Snake::hitWall()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    if (headX <= 0 || headX >= this->mGameBoardWidth - 1)
    {
        return true;
    }
    if (headY <= 0 || headY >= this->mGameBoardHeight - 1)
    {
        return true;
    }
    return false;
}

/*
 * The snake head is overlapping with its body
 */
bool Snake::hitSelf()
{
    SnakeBody& head = this->mSnake[0];
    // Exclude the snake head
    for (int i = 1; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == head)
        {
            return true;
        }
    }
    return false;
}

bool Snake::hitObstacle()
{
    SnakeBody& head = this->mSnake[0];
    for (SnakeBody oneObstacle: Obstacle) {
        if (head == oneObstacle && !invincibility)
        {
            return true;
        }
    }
    return false;
}

int Snake::touchThing()
{
    SnakeBody newHead = this->createNewHead();
    for (SnakeBody oneFood: this->mFood) {
        if (oneFood == newHead) {
            this->foodsound->play();
            return 1;
        }
    }
    if (this->Gate[0] == newHead)
    {
        this->travelsound->play();
        return 2;
    }
    else if (this->Gate[1] == newHead)
    {
        this->travelsound->play();
        return 3;
    }
    else if (this->Gate[2] == newHead)
    {
        this->travelsound->play();
        return 4;
    }
    else if (this->Gate[3] == newHead)
    {
        this->travelsound->play();
        return 5;
    }
    else if (this->Gate[4] == newHead)
    {
        this->travelsound->play();
        return 6;
    }
    else if (this->Gate[5] == newHead)
    {
        this->travelsound->play();
        return 7;
    }
    else
    {
        return 0;
    }
}

int Snake::touchFoodIndex(int &x)
{
    SnakeBody newHead = mSnake[0];
    bool op = false;
    int k = -1;
    for (int i = 0; i < this->mFood.size(); i++)
    {
        if (this->mFood[i]== newHead)
        {
            op = true;
            k = i;
            break;
        }
    }
    if (op == true)
    {
        x = k;
        return 1;
    }
    else
    {
        return 0;
    }
}

void Snake::senseFood(SnakeBody food, int score)
{
    this->mFood.push_back(food);
    this->mScore.push_back(score);
}

void Snake::senseFood(std::vector<SnakeBody> mfood, std::vector<int> mscore)
{
    this->mFood = mfood;
    this->mScore = mscore;
}

void Snake::senseGate(SnakeBody ga1, SnakeBody ga2, SnakeBody ga3, SnakeBody ga4, SnakeBody ga5, SnakeBody ga6)
{
    this->Gate.push_back(ga1);
    this->Gate.push_back(ga2);
    this->Gate.push_back(ga3);
    this->Gate.push_back(ga4);
    this->Gate.push_back(ga5);
    this->Gate.push_back(ga6);
}

//以vector形式增添障碍物
void Snake::senseObstacle(std::vector<SnakeBody> mObstacle)
{
    for (int i = 0; i < mObstacle.size(); i++) {
        this->Obstacle.push_back(mObstacle[i]);
    }
}

//以SnakeBody形式增添障碍物
void Snake::senseObstacle(SnakeBody oneObstacle)
{
    this->Obstacle.push_back(oneObstacle);
}

//设置无敌模式
void Snake::setInvin()
{
    //如果技能还在冷却状态，无法设置
    if (this->invin_present_CD) return;

    //设置无敌模式为真，重置无敌CD和无敌时长
    this->dingsound->play();
    this->invincibility = true;
    this->invin_present_CD = this->invin_whole_CD;
    this->invin_present_time = this->invin_whole_time;
}

std::vector<SnakeBody>& Snake::getSnake()
{
    return this->mSnake;
}

bool Snake::changeDirection(Direction newDirection)
{
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            if (newDirection == Direction::Left || newDirection == Direction::Right)
            {
                this->movesound->play();
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Down:
        {
            if (newDirection == Direction::Left || newDirection == Direction::Right)
            {
                this->movesound->play();
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Left:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Down)
            {
                this->movesound->play();
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Right:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Down)
            {
                this->movesound->play();
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

SnakeBody Snake::createNewHead()
{
    SnakeBody a = this->mSnake[0];
    int x = a.getX();
    int y = a.getY();
    Direction b = this->mDirection;

    if (b == Direction::Down)
        this->mSnake.insert(mSnake.begin(),SnakeBody(x, y + 1));
    else if (b == Direction::Left)
        this->mSnake.insert(mSnake.begin(),SnakeBody(x - 1, y));
    else if (b == Direction::Right)
        this->mSnake.insert(mSnake.begin(),SnakeBody(x + 1, y));
    else
        this->mSnake.insert(mSnake.begin(),SnakeBody(x, y - 1));

    SnakeBody newHead = this->mSnake[0];
    return newHead;
}

/*
 * If eat food, return true, otherwise return false
 */
bool Snake::moveFoward()
{
    /*
		 * TODO
		 * move the snake forward.
     * If eat food, return true, otherwise return false
     */
     //无敌状态设置
     this->dicrCD();
     this->dicrInvTime(); //减少冷却CD和无敌时长（如果这两个数字此时大于0）
     this->resetInvin();  //如果无敌时间结束，那就终止无敌状态

    int flag = this->touchThing();
    Direction b = this->mDirection;

    if (flag == 1)
        return true;
    else if (flag == 2)
    {
        int x = this->Gate[1].getX();
        int y = this->Gate[1].getY();

        if (b == Direction::Down)
            this->mSnake[0] = SnakeBody(x,y+1);
        else if (b==Direction::Left)
            this->mSnake[0] = SnakeBody(x-1,y);
        else if (b==Direction::Right)
            this->mSnake[0] = SnakeBody(x+1,y);
        else
            this->mSnake[0] = SnakeBody(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 3)
    {
        int x = this->Gate[0].getX();
        int y = this->Gate[0].getY();

        if (b == Direction::Down)
            this->mSnake[0] = SnakeBody(x,y+1);
        else if (b==Direction::Left)
            this->mSnake[0] = SnakeBody(x-1,y);
        else if (b==Direction::Right)
            this->mSnake[0] = SnakeBody(x+1,y);
        else
            this->mSnake[0] = SnakeBody(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 4)
    {
        int x = this->Gate[3].getX();
        int y = this->Gate[3].getY();

        if (b == Direction::Down)
            this->mSnake[0] = SnakeBody(x,y+1);
        else if (b==Direction::Left)
            this->mSnake[0] = SnakeBody(x-1,y);
        else if (b==Direction::Right)
            this->mSnake[0] = SnakeBody(x+1,y);
        else
            this->mSnake[0] = SnakeBody(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 5)
    {
        int x = this->Gate[2].getX();
        int y = this->Gate[2].getY();

        if (b == Direction::Down)
            this->mSnake[0] = SnakeBody(x,y+1);
        else if (b==Direction::Left)
            this->mSnake[0] = SnakeBody(x-1,y);
        else if (b==Direction::Right)
            this->mSnake[0] = SnakeBody(x+1,y);
        else
            this->mSnake[0] = SnakeBody(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 6)
    {
        int x = this->Gate[5].getX();
        int y = this->Gate[5].getY();

        if (b == Direction::Down)
            this->mSnake[0] = SnakeBody(x,y+1);
        else if (b==Direction::Left)
            this->mSnake[0] = SnakeBody(x-1,y);
        else if (b==Direction::Right)
            this->mSnake[0] = SnakeBody(x+1,y);
        else
            this->mSnake[0] = SnakeBody(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 7)
    {
        int x = this->Gate[4].getX();
        int y = this->Gate[4].getY();

        if (b == Direction::Down)
            this->mSnake[0] = SnakeBody(x,y+1);
        else if (b==Direction::Left)
            this->mSnake[0] = SnakeBody(x-1,y);
        else if (b==Direction::Right)
            this->mSnake[0] = SnakeBody(x+1,y);
        else
            this->mSnake[0] = SnakeBody(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 0)
    {
        this->mSnake.pop_back();
        return false;
    }
}

bool Snake::checkCollision()
{
    if (this->hitWall() || this->hitSelf() || this->hitObstacle())
    {
        return true;
    }
    else
    {
        return false;
    }
}


int Snake::getLength()
{
    return this->mSnake.size();
}

