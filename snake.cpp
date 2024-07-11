#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake.h"


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


int Snake::touchThing()
{
    SnakeBody newHead = this->createNewHead();
    if (this->mFood == newHead)
    {
        return 1;
    }
    else if (this->Gate[0] == newHead)
    {
        return 2;
    }
    else if (this->Gate[1] == newHead)
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

void Snake::senseFood(SnakeBody food)
{
    this->mFood = food;
}

void Snake::senseGate(SnakeBody ga1, SnakeBody ga2)
{
    this->Gate[0]=ga1;
    this->Gate[1]=ga2;
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
    else if (flag == 0)
    {
        this->mSnake.pop_back();
        return false;
    }
}

bool Snake::checkCollision()
{
    if (this->hitWall() || this->hitSelf())
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

