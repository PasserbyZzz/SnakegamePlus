#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake_great.h"
#include "color.h"

SnakeBody_great::SnakeBody_great()
{
}


SnakeBody_great::SnakeBody_great(int x, int y): mX(x), mY(y)
{
}

int SnakeBody_great::getX() const
{
    return mX;
}

int SnakeBody_great::getY() const
{
    return mY;
}

bool SnakeBody_great::operator == (const SnakeBody_great& snakeBody)
{
    return (this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY());
}

Snake_great::Snake_great(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
    this->travelsound = gameSound("travel_through_wormhole.wav");
    this->foodsound = gameSound("game_bonus.wav");
    this->movesound = gameSound("snake_move.wav");
    this->dingsound = gameSound("ding.wav");

}

Snake_great::~Snake_great()
{
    delete travelsound;
    delete foodsound;
    delete movesound;
}

void Snake_great::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void Snake_great::initializeSnake()
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
            this->mDirection = Direction_great::Up;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBody_great(centerX, centerY + i));
            }
            break;
        case 1:
            this->mDirection = Direction_great::Down;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBody_great(centerX, centerY - i));
            }
            break;
        case 2:
            this->mDirection = Direction_great::Left;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBody_great(centerX + 1, centerY));
            }
            break;
        case 3:
            this->mDirection = Direction_great::Right;
            for (int i = 0; i < this->mInitialSnakeLength; i ++){
                this->mSnake.push_back(SnakeBody_great(centerX - 1, centerY));
            }
            break;
    }
}

bool Snake_great::isPartOfSnake(int x, int y)
{
    SnakeBody_great temp = SnakeBody_great(x, y);
    for (int i = 0; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == temp)
        {
            return true;
        }
    }
    return false;
}

int Snake_great::touchFoodIndex(int &x)
{
    SnakeBody_great newHead = mSnake[0];
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


bool Snake_great::isPartOfFood(int x, int y)
{
    SnakeBody_great temp = SnakeBody_great(x, y);
    for (SnakeBody_great oneFood: this->mFood) {
        if (oneFood == temp)
            return true;
    }
    return false;
}


//changed by qjc
bool Snake_great::isPartOfObstacle(int x, int y)
{
    SnakeBody_great temp = SnakeBody_great(x, y);
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
bool Snake_great::hitWall()
{
    bool flag(false);
    SnakeBody_great& head = this->mSnake[0];
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
bool Snake_great::hitSelf()
{
    SnakeBody_great& head = this->mSnake[0];
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
bool Snake_great::hitObstacle()
{
    SnakeBody_great& head = this->mSnake[0];
    for (SnakeBody_great oneObstacle: Obstacle) {
        if (head == oneObstacle && !invincibility)
        {
            return true;
        }
    }
    return false;
}


int Snake_great::touchThing()
{
    SnakeBody_great newHead = this->createNewHead();
    for (SnakeBody_great oneFood: this->mFood) {
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

void Snake_great::senseFood(std::vector<SnakeBody_great> mfood, std::vector<int> mscore)
{
    this->mFood = mfood;
    this->mScore = mscore;
}

void Snake_great::senseFood(SnakeBody_great food, int score)
{
    this->mFood.push_back(food);
    this->mScore.push_back(score);
}

void Snake_great::senseGate(SnakeBody_great ga1, SnakeBody_great ga2, SnakeBody_great ga3, SnakeBody_great ga4, SnakeBody_great ga5, SnakeBody_great ga6)
{
    this->Gate.push_back(ga1);
    this->Gate.push_back(ga2);
    this->Gate.push_back(ga3);
    this->Gate.push_back(ga4);
    this->Gate.push_back(ga5);
    this->Gate.push_back(ga6);
}

//��vector��ʽ�����ϰ���
//changed by qjc:
void Snake_great::senseObstacle(std::vector<SnakeBody_great> mObstacle)
{
    for (int i = 0; i < mObstacle.size(); i++) {
        this->Obstacle.push_back(mObstacle[i]);
    }
}

//��SnakeBody��ʽ�����ϰ���
//changed by qjc
void Snake_great::senseObstacle(SnakeBody_great oneObstacle)
{
    this->Obstacle.push_back(oneObstacle);
}


//�����޵�ģʽ
//changed by qjc
void Snake_great::setInvin()
{
    //������ܻ�����ȴ״̬���޷�����
    if (this->invin_present_CD) return;

    //�����޵�ģʽΪ�棬�����޵�CD���޵�ʱ��
    this->invincibility = true;
    this->dingsound->play();
    this->invin_present_CD = this->invin_whole_CD;
    this->invin_present_time = this->invin_whole_time;
}

std::vector<SnakeBody_great>& Snake_great::getSnake()
{
    return this->mSnake;
}

bool Snake_great::changeDirection(Direction_great newDirection)
{
    switch (this->mDirection)
    {
        case Direction_great::Up:
        {
            if (newDirection == Direction_great::Left || newDirection == Direction_great::Right)
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
        case Direction_great::Down:
        {
            if (newDirection == Direction_great::Left || newDirection == Direction_great::Right)
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
        case Direction_great::Left:
        {
            if (newDirection == Direction_great::Up || newDirection == Direction_great::Down)
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
        case Direction_great::Right:
        {
            if (newDirection == Direction_great::Up || newDirection == Direction_great::Down)
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

SnakeBody_great Snake_great::createNewHead()
{
    SnakeBody_great a = this->mSnake[0];
    int x = a.getX();
    int y = a.getY();
    Direction_great b = this->mDirection;

    if (b == Direction_great::Down)
        this->mSnake.insert(mSnake.begin(),SnakeBody_great(x, y + 1));
    else if (b == Direction_great::Left)
        this->mSnake.insert(mSnake.begin(),SnakeBody_great(x - 1, y));
    else if (b == Direction_great::Right)
        this->mSnake.insert(mSnake.begin(),SnakeBody_great(x + 1, y));
    else
        this->mSnake.insert(mSnake.begin(),SnakeBody_great(x, y - 1));

    SnakeBody_great newHead = this->mSnake[0];
    return newHead;
}

SnakeBody_great Snake_great::createNewHead(int x, int y)
{
    SnakeBody_great newHead = SnakeBody_great(x, y);
    this->mSnake.insert(mSnake.begin(),newHead);
    return newHead;
}

/*
 * If eat food, return true, otherwise return false
 */
bool Snake_great::moveFoward()
{
    /*
		 * TODO
		 * move the snake forward.
     * If eat food, return true, otherwise return false
     */

     //changed by qjc
     //�޵�״̬����
     this->dicrCD();
     this->dicrInvTime(); //������ȴCD���޵�ʱ����������������ִ�ʱ����0��
     this->resetInvin();  //����޵�ʱ��������Ǿ���ֹ�޵�״̬


    int flag = this->touchThing();
    Direction_great b = this->mDirection;

    if (flag == 1)
        return true;
    else if (flag == 2)
    {
        int x = this->Gate[1].getX();
        int y = this->Gate[1].getY();

        if (b == Direction_great::Down)
            this->mSnake[0] = SnakeBody_great(x,y+1);
        else if (b==Direction_great::Left)
            this->mSnake[0] = SnakeBody_great(x-1,y);
        else if (b==Direction_great::Right)
            this->mSnake[0] = SnakeBody_great(x+1,y);
        else
            this->mSnake[0] = SnakeBody_great(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 3)
    {
        int x = this->Gate[0].getX();
        int y = this->Gate[0].getY();

        if (b == Direction_great::Down)
            this->mSnake[0] = SnakeBody_great(x,y+1);
        else if (b==Direction_great::Left)
            this->mSnake[0] = SnakeBody_great(x-1,y);
        else if (b==Direction_great::Right)
            this->mSnake[0] = SnakeBody_great(x+1,y);
        else
            this->mSnake[0] = SnakeBody_great(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 4)
    {
        int x = this->Gate[3].getX();
        int y = this->Gate[3].getY();

        if (b == Direction_great::Down)
            this->mSnake[0] = SnakeBody_great(x,y+1);
        else if (b==Direction_great::Left)
            this->mSnake[0] = SnakeBody_great(x-1,y);
        else if (b==Direction_great::Right)
            this->mSnake[0] = SnakeBody_great(x+1,y);
        else
            this->mSnake[0] = SnakeBody_great(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 5)
    {
        int x = this->Gate[2].getX();
        int y = this->Gate[2].getY();

        if (b == Direction_great::Down)
            this->mSnake[0] = SnakeBody_great(x,y+1);
        else if (b==Direction_great::Left)
            this->mSnake[0] = SnakeBody_great(x-1,y);
        else if (b==Direction_great::Right)
            this->mSnake[0] = SnakeBody_great(x+1,y);
        else
            this->mSnake[0] = SnakeBody_great(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 6)
    {
        int x = this->Gate[5].getX();
        int y = this->Gate[5].getY();

        if (b == Direction_great::Down)
            this->mSnake[0] = SnakeBody_great(x,y+1);
        else if (b==Direction_great::Left)
            this->mSnake[0] = SnakeBody_great(x-1,y);
        else if (b==Direction_great::Right)
            this->mSnake[0] = SnakeBody_great(x+1,y);
        else
            this->mSnake[0] = SnakeBody_great(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 7)
    {
        int x = this->Gate[4].getX();
        int y = this->Gate[4].getY();

        if (b == Direction_great::Down)
            this->mSnake[0] = SnakeBody_great(x,y+1);
        else if (b==Direction_great::Left)
            this->mSnake[0] = SnakeBody_great(x-1,y);
        else if (b==Direction_great::Right)
            this->mSnake[0] = SnakeBody_great(x+1,y);
        else
            this->mSnake[0] = SnakeBody_great(x,y-1);

        this->mSnake.pop_back();
        return false;
    }
    else if (flag == 0)
    {
        this->mSnake.pop_back();
        return false;
    }
}

//changed by qjc
bool Snake_great::checkCollision()
{
    bool flag(false);

    if (this->hitWall()) {
        flag = false;
    }
    if (this->hitObstacle())
    {
        setInvin();
        flag = false;
    }
    if (this->hitSelf()) {
        flag = false;
    }
    if (getLength() < 1 || getLength() > 30) {
        flag = true;
    }
    return flag;
}


int Snake_great::getLength()
{
    return this->mSnake.size();
}


