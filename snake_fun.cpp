#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake_fun.h"
#include "color.h"

SnakeBodyFun::SnakeBodyFun()
{
}


SnakeBodyFun::SnakeBodyFun(int x, int y): mX(x), mY(y)
{
}

int SnakeBodyFun::getX() const
{
    return mX;
}

int SnakeBodyFun::getY() const
{
    return mY;
}

bool SnakeBodyFun::operator == (const SnakeBodyFun& snakeBody)
{
    return (this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY());
}

SnakeFun::SnakeFun(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
    this->foodsound = gameSound("game_bonus.wav");
    this->dingsound = gameSound("ding.wav");
}

SnakeFun::~SnakeFun()
{
    delete foodsound;
}

void SnakeFun::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void SnakeFun::initializeSnake()
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
            this->mDirection = DirectionFun::Up;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBodyFun(centerX, centerY + i));
            }
            break;
        case 1:
            this->mDirection = DirectionFun::Down;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBodyFun(centerX, centerY - i));
            }
            break;
        case 2:
            this->mDirection = DirectionFun::Left;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBodyFun(centerX + 1, centerY));
            }
            break;
        case 3:
            this->mDirection = DirectionFun::Right;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBodyFun(centerX - 1, centerY));
            }
            break;
    }
}

bool SnakeFun::isPartOfSnake(int x, int y)
{
    SnakeBodyFun temp = SnakeBodyFun(x, y);
    for (int i = 0; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == temp)
        {
            return true;
        }
    }
    return false;
}

int SnakeFun::touchFoodIndex(int &x)
{
    SnakeBodyFun newHead = mSnake[0];
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
    /*
    else if (this->Gate[0] == newHead)
    {
        return 2;
    }
    else if (this->Gate[1] == newHead)
    {
        return 3;
    }
    */
    else
    {
        return 0;
    }
}


bool SnakeFun::isPartOfFood(int x, int y)
{
    SnakeBodyFun temp = SnakeBodyFun(x, y);
    for (SnakeBodyFun oneFood: this->mFood) {
        if (oneFood == temp)
            return true;
    }
    return false;
}


//changed by qjc
bool SnakeFun::isPartOfObstacle(int x, int y)
{
    SnakeBodyFun temp = SnakeBodyFun(x, y);
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
bool SnakeFun::hitWall()
{
    bool flag(false);
    SnakeBodyFun& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    if (headX <= 0 || headX >= this->mGameBoardWidth - 1) {
        flag = true;
    }
    if (headY <= 0 || headY >= this->mGameBoardHeight - 1) {
        flag = true;
    }

    if (headX <= 0) {
        createNewHead(mGameBoardWidth-2, headY);
        mSnake.pop_back();
    }
    else if (headX >= this->mGameBoardWidth - 1) {
        createNewHead(1, headY);
        mSnake.pop_back();
    }
    else if (headY <= 0) {
        createNewHead(headX, mGameBoardHeight-2);
        mSnake.pop_back();
    }
    else if (headY >= this->mGameBoardHeight - 1) {
        createNewHead(headX, 1);
        mSnake.pop_back();
    }



    return flag;
}

/*
 * The snake head is overlapping with its body
 */
bool SnakeFun::hitSelf()
{
    SnakeBodyFun& head = this->mSnake[0];
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

/*
 * Assumption:
 * Only the head would hit Obstacle.
 */
 //changed by qjc
bool SnakeFun::hitObstacle()
{
    SnakeBodyFun& head = this->mSnake[0];
    for (SnakeBodyFun oneObstacle: Obstacle) {
        if (head == oneObstacle && !invincibility)
        {
            return true;
        }
    }
    return false;
}


int SnakeFun::touchThing()
{
    SnakeBodyFun newHead = this->createNewHead();
    for (SnakeBodyFun oneFood: this->mFood) {
        if (oneFood == newHead) {
            this->foodsound->play();
            return 1;
        }
    }

    /*
    if (this->Gate[0] == newHead)
    {
        return 2;
    }
    else if (this->Gate[1] == newHead)
    {
        return 3;
    }
    else if (this->Gate[2] == newHead)
    {
        return 4;
    }
    else if (this->Gate[3] == newHead)
    {
        return 5;
    }
    else if (this->Gate[4] == newHead)
    {
        return 6;
    }
    else if (this->Gate[5] == newHead)
    {
        return 7;
    }
    */

    return 0;

}

void SnakeFun::senseFood(std::vector<SnakeBodyFun> mfood, std::vector<int> mscore)
{
    this->mFood = mfood;
    this->mScore = mscore;
}

void SnakeFun::senseFood(SnakeBodyFun food, int score)
{
    this->mFood.push_back(food);
    this->mScore.push_back(score);
}

/*
void SnakeFun::senseGate(SnakeBody ga1, SnakeBody ga2, SnakeBody ga3, SnakeBody ga4, SnakeBody ga5, SnakeBody ga6)
{
    this->Gate.push_back(ga1);
    this->Gate.push_back(ga2);
    this->Gate.push_back(ga3);
    this->Gate.push_back(ga4);
    this->Gate.push_back(ga5);
    this->Gate.push_back(ga6);
}
*/

//以vector形式增添障碍物
//changed by qjc:
void SnakeFun::senseObstacle(std::vector<SnakeBodyFun> mObstacle)
{
    for (int i = 0; i < mObstacle.size(); i++) {
        this->Obstacle.push_back(mObstacle[i]);
    }
}

//以SnakeBody形式增添障碍物
//changed by qjc
void SnakeFun::senseObstacle(SnakeBodyFun oneObstacle)
{
    this->Obstacle.push_back(oneObstacle);
}


//设置无敌模式
//changed by qjc
void SnakeFun::setInvin()
{
    //如果技能还在冷却状态，无法设置
    if (this->invin_present_CD) return;

    //设置无敌模式为真，重置无敌CD和无敌时长
    this->invincibility = true;
    this->dingsound->play();
    this->invin_present_CD = this->invin_whole_CD;
    this->invin_present_time = this->invin_whole_time;
}

std::vector<SnakeBodyFun>& SnakeFun::getSnake()
{
    return this->mSnake;
}

bool SnakeFun::changeDirection(DirectionFun newDirection)
{
    switch (this->mDirection)
    {
        case DirectionFun::Up:
        {
            if (newDirection == DirectionFun::Left || newDirection == DirectionFun::Right)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case DirectionFun::Down:
        {
            if (newDirection == DirectionFun::Left || newDirection == DirectionFun::Right)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case DirectionFun::Left:
        {
            if (newDirection == DirectionFun::Up || newDirection == DirectionFun::Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case DirectionFun::Right:
        {
            if (newDirection == DirectionFun::Up || newDirection == DirectionFun::Down)
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

SnakeBodyFun SnakeFun::createNewHead()
{
    SnakeBodyFun a = this->mSnake[0];
    int x = a.getX();
    int y = a.getY();
    DirectionFun b = this->mDirection;

    if (b == DirectionFun::Down)
        this->mSnake.insert(mSnake.begin(),SnakeBodyFun(x, y + 1));
    else if (b == DirectionFun::Left)
        this->mSnake.insert(mSnake.begin(),SnakeBodyFun(x - 1, y));
    else if (b == DirectionFun::Right)
        this->mSnake.insert(mSnake.begin(),SnakeBodyFun(x + 1, y));
    else
        this->mSnake.insert(mSnake.begin(),SnakeBodyFun(x, y - 1));

    SnakeBodyFun newHead = this->mSnake[0];
    return newHead;
}

SnakeBodyFun SnakeFun::createNewHead(int x, int y)
{
    SnakeBodyFun newHead = SnakeBodyFun(x, y);
    this->mSnake.insert(mSnake.begin(),newHead);
    return newHead;
}

/*
 * If eat food, return true, otherwise return false
 */
bool SnakeFun::moveFoward()
{
    /*
		 * TODO
		 * move the snake forward.
     * If eat food, return true, otherwise return false
     */

     //changed by qjc
     //无敌状态设置
     this->dicrCD();
     this->dicrInvTime(); //减少冷却CD和无敌时长（如果这两个数字此时大于0）
     this->resetInvin();  //如果无敌时间结束，那就终止无敌状态


    int flag = this->touchThing();
    DirectionFun b = this->mDirection;

    if (flag == 1)
        return true;
    /*
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
    */
    else if (flag == 0)
    {
        this->mSnake.pop_back();
        return false;
    }
}

//changed by qjc
bool SnakeFun::checkCollision()
{
    bool flag(false);
    if (this->hitWall() || this->hitSelf()) {
        flag = false; //娱乐模式如果要增加长度的话就不要自己杀自己了
    }
    if (this->hitObstacle())
    {
        //setInvin();
        flag = true;
    }
    return flag;
}


int SnakeFun::getLength()
{
    return this->mSnake.size();
}


