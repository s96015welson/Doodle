#include "Player.h"
#include <iostream>
#include <QGraphicsScene>
#include <typeinfo>
#include "Stair.h"
#include "Game.h"
#include "Parameter.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>
extern Game *game; // there is an external global object called game

Player::Player(QGraphicsItem *parent, const char *img) : QObject(), QGraphicsPixmapItem(parent)
{
    setPos(PLAYER_START_POSITION_X, PLAYER_START_POSITION_Y); // generalize to always be in the middle top of screen
    setZValue(PLAYER_ITEM_ORDER);
    setPixmap(QPixmap(img).scaled(DOODLE_WIDTH, DOODLE_HEIGHT));
}

void Player::moveLeft()
{
    if (x() + DOODLE_WIDTH - left_moving_speed > 0)
        setPos(x() - left_moving_speed, y());
    else
        setPos(x() - left_moving_speed + SCREEN_WIDTH, y());
}

void Player::moveRight()
{
    if (x() + right_moving_speed < SCREEN_WIDTH)
        setPos(x() + right_moving_speed, y());
    else
        setPos(x() + right_moving_speed - SCREEN_WIDTH, y());
}

void Player::fall(int fall_speed)
{
    setPos(x(), y() + fall_speed);
}

void Player::rise()
{
    setPos(x(), y() - rising_speed);
}

void Player::jump()
{
    //
}
void Player::fly()
{
    //
}

bool Player::die()
{
    return (y() + height() * 0.5) >= SCREEN_HEIGHT;
}

int Player::width()
{
    return DOODLE_WIDTH;
}

int Player::height()
{
    return DOODLE_HEIGHT;
}

int Player::pos_Left()
{
    return x();
}
int Player::pos_Right()
{
    return x() + DOODLE_WIDTH;
}
int Player::pos_Up()
{
    return y();
}
int Player::pos_Down()
{
    return y() + DOODLE_HEIGHT;
}