#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake_pair.h"
#include "color.h"

SnakeBodyPair::SnakeBodyPair()
{
}


SnakeBodyPair::SnakeBodyPair(int x, int y): mX(x), mY(y)
{
}

int SnakeBodyPair::getX() const
{
    return mX;
}

int SnakeBodyPair::getY() const
{
    return mY;
}

bool SnakeBodyPair::operator == (const SnakeBodyPair& snakeBody)
{
    // TODO overload the == operator for SnakeBody comparision.
    if (this->getX()==snakeBody.getX() && this->getY()==snakeBody.getY())
        return true;

    return false;
}

SnakePair::SnakePair(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
    num = -num;

    this->travelsound = gameSound("travel_through_wormhole.wav");
    this->foodsound = gameSound("game_bonus.wav");
    this->movesound = gameSound("snake_move.wav");
}

SnakePair::~SnakePair()
{
    //delete travelsound;
    //delete foodsound;
    //delete movesound;
}

void SnakePair::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void SnakePair::initializeSnake()
{
    // Instead of using a random initialization algorithm
    // We always put the snake at the center of the game mWindows

    int centerX = this->mGameBoardWidth / 2 - num;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialSnakeLength; i ++)
    {
        this->mSnake.push_back(SnakeBodyPair(centerX, centerY + i));
    }
    this->mDirection = DirectionPair::Up;
}

bool SnakePair::isPartOfSnake(int x, int y)
{
    // TODO check if a given point with axis x, y is on the body of the snake.
    for (int i = 0; i < this->getLength(); i ++)
    {
        if (this->mSnake[i]==SnakeBodyPair(x,y))
            return true;
    }

    return false;
}

bool SnakePair::isPartOfFood(int x, int y)
{
    SnakeBodyPair temp = SnakeBodyPair(x, y);
    for (SnakeBodyPair oneFood: this->mFood) {
        if (oneFood == temp)
            return true;
    }
    return false;
}

bool SnakePair::isPartOfObstacle(int x, int y)
{
    SnakeBodyPair temp = SnakeBodyPair(x, y);
    for (int i = 0; i < this->Obstacle.size(); i ++)
    {
        if (this->Obstacle[i] == temp)
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
bool SnakePair::hitWall()
{
    // TODO check if the snake has hit the wall
    SnakeBodyPair& head = this->mSnake[0];
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
bool SnakePair::hitSelf()
{
    // TODO check if the snake has hit itself.
    SnakeBodyPair temp = this->mSnake[0];
    int a = temp.getX();
    int b = temp.getY();

    switch (this->mDirection) {
        case DirectionPair::Up:
            if (this->isPartOfSnake(a, b - 1)) return true;
            break;
        case DirectionPair::Down:
            if (this->isPartOfSnake(a, b + 1)) return true;
            break;
        case DirectionPair::Left:
            if (this->isPartOfSnake(a - 1, b)) return true;
            break;
        case DirectionPair::Right:
            if (this->isPartOfSnake(a + 1, b)) return true;
            break;
    }
    return false;
}

bool SnakePair::hitObstacle()
{
    SnakeBodyPair& head = this->mSnake[0];
    for (SnakeBodyPair oneObstacle: Obstacle) {
        if (head == oneObstacle)
        {
            return true;
        }
    }
    return false;
}

bool SnakePair::hitOther(SnakePair other)
{
    SnakeBodyPair temp = this->mSnake[0];
    int a = temp.getX();
    int b = temp.getY();

    switch (this->mDirection) {
        case DirectionPair::Up:
            if (other.isPartOfSnake(a, b - 1)) return true;
            break;
        case DirectionPair::Down:
            if (other.isPartOfSnake(a, b + 1)) return true;
            break;
        case DirectionPair::Left:
            if (other.isPartOfSnake(a - 1, b)) return true;
            break;
        case DirectionPair::Right:
            if (other.isPartOfSnake(a + 1, b)) return true;
            break;
    }
    return false;
}

int SnakePair::touchThing(int &x)
{
    SnakeBodyPair newHead = this->createNewHead();
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

        x = k;

    if (op == true)
    {
        this->foodsound->play();
        return 1;
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

void SnakePair::eraseFood(int x)
{
    this->mFood.erase(this->mFood.begin()+x);
}

void SnakePair::senseFood(SnakeBodyPair food)
{
    this->mFood.push_back(food);
}

void SnakePair::senseGate(SnakeBodyPair ga1, SnakeBodyPair ga2, SnakeBodyPair ga3, SnakeBodyPair ga4, SnakeBodyPair ga5, SnakeBodyPair ga6)
{
    this->Gate.push_back(ga1);
    this->Gate.push_back(ga2);
    this->Gate.push_back(ga3);
    this->Gate.push_back(ga4);
    this->Gate.push_back(ga5);
    this->Gate.push_back(ga6);
}

void SnakePair::senseObstacle(std::vector<SnakeBodyPair> mObstacle)
{
    for (int i = 0; i < mObstacle.size(); i++) {
        this->Obstacle.push_back(mObstacle[i]);
    }
}

void SnakePair::senseObstacle(SnakeBodyPair oneObstacle)
{
    this->Obstacle.push_back(oneObstacle);
}

std::vector<SnakeBodyPair>& SnakePair::getSnake()
{
    return this->mSnake;
}

bool SnakePair::changeDirection(DirectionPair newDirection)
{
    switch (this->mDirection)
    {
        case DirectionPair::Up:
        {
            if (newDirection == DirectionPair::Left || newDirection == DirectionPair::Right)
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
        case DirectionPair::Down:
        {
            if (newDirection == DirectionPair::Left || newDirection == DirectionPair::Right)
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
        case DirectionPair::Left:
        {
            if (newDirection == DirectionPair::Up || newDirection == DirectionPair::Down)
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
        case DirectionPair::Right:
        {
            if (newDirection == DirectionPair::Up || newDirection == DirectionPair::Down)
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


SnakeBodyPair SnakePair::createNewHead()
{
    SnakeBodyPair a = this->mSnake[0];
    int x = a.getX();
    int y = a.getY();
    DirectionPair b = this->mDirection;

    if (b == DirectionPair::Down)
        this->mSnake.insert(mSnake.begin(),SnakeBodyPair(x,y+1));
    else if (b==DirectionPair::Left)
        this->mSnake.insert(mSnake.begin(),SnakeBodyPair(x-1,y));
    else if (b==DirectionPair::Right)
        this->mSnake.insert(mSnake.begin(),SnakeBodyPair(x+1,y));
    else
        this->mSnake.insert(mSnake.begin(),SnakeBodyPair(x,y-1));

    SnakeBodyPair newHead = this->mSnake[0];
    return newHead;
}

bool SnakePair::moveFoward(int &m)
{
    int k;
    int flag = this->touchThing(k);
    DirectionPair b = this->mDirection;

    m = k;

    if (flag == 1)
    {
        this->mFood.erase(mFood.begin()+k);
        return true;
    }
    else if (flag == 2)
    {
        int x = this->Gate[1].getX();
        int y = this->Gate[1].getY();

        if (b == DirectionPair::Down)
            this->mSnake[0] = SnakeBodyPair(x,y+1);
        else if (b==DirectionPair::Left)
            this->mSnake[0] = SnakeBodyPair(x-1,y);
        else if (b==DirectionPair::Right)
            this->mSnake[0] = SnakeBodyPair(x+1,y);
        else
            this->mSnake[0] = SnakeBodyPair(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 3)
    {
        int x = this->Gate[0].getX();
        int y = this->Gate[0].getY();

        if (b == DirectionPair::Down)
            this->mSnake[0] = SnakeBodyPair(x,y+1);
        else if (b==DirectionPair::Left)
            this->mSnake[0] = SnakeBodyPair(x-1,y);
        else if (b==DirectionPair::Right)
            this->mSnake[0] = SnakeBodyPair(x+1,y);
        else
            this->mSnake[0] = SnakeBodyPair(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 4)
    {
        int x = this->Gate[3].getX();
        int y = this->Gate[3].getY();

        if (b == DirectionPair::Down)
            this->mSnake[0] = SnakeBodyPair(x,y+1);
        else if (b==DirectionPair::Left)
            this->mSnake[0] = SnakeBodyPair(x-1,y);
        else if (b==DirectionPair::Right)
            this->mSnake[0] = SnakeBodyPair(x+1,y);
        else
            this->mSnake[0] = SnakeBodyPair(x,y-1);


        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 5)
    {
        int x = this->Gate[2].getX();
        int y = this->Gate[2].getY();

        if (b == DirectionPair::Down)
            this->mSnake[0] = SnakeBodyPair(x,y+1);
        else if (b==DirectionPair::Left)
            this->mSnake[0] = SnakeBodyPair(x-1,y);
        else if (b==DirectionPair::Right)
            this->mSnake[0] = SnakeBodyPair(x+1,y);
        else
            this->mSnake[0] = SnakeBodyPair(x,y-1);


        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 6)
    {
        int x = this->Gate[5].getX();
        int y = this->Gate[5].getY();

        if (b == DirectionPair::Down)
            this->mSnake[0] = SnakeBodyPair(x,y+1);
        else if (b==DirectionPair::Left)
            this->mSnake[0] = SnakeBodyPair(x-1,y);
        else if (b==DirectionPair::Right)
            this->mSnake[0] = SnakeBodyPair(x+1,y);
        else
            this->mSnake[0] = SnakeBodyPair(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 7)
    {
        int x = this->Gate[4].getX();
        int y = this->Gate[4].getY();

        if (b == DirectionPair::Down)
            this->mSnake[0] = SnakeBodyPair(x,y+1);
        else if (b==DirectionPair::Left)
            this->mSnake[0] = SnakeBodyPair(x-1,y);
        else if (b==DirectionPair::Right)
            this->mSnake[0] = SnakeBodyPair(x+1,y);
        else
            this->mSnake[0] = SnakeBodyPair(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 0)
    {
        this->mSnake.pop_back();
        return false;
    }
}

bool SnakePair::checkCollision(SnakePair other)
{
    if (this->hitWall() || this->hitSelf() || this->hitOther(other) || this->hitObstacle())
    {
        return true;
    }
    else
    {
        return false;
    }
}


int SnakePair::getLength()
{
    return this->mSnake.size();
}

DirectionPair SnakePair::getDirection()
{
    return this->mDirection;
}

