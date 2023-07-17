#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <cstdlib>
#include <QPixmap>
#include <math.h>
#include "Game.h"
#include "Parameter.h"
#include "Bullet.h"
#include <iostream>

extern Game *game;

Bullet::Bullet(int doodleX, int doodleY, int targetX, int targetY, QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{

    initial_pos_x = doodleX + 0.5 * DOODLE_WIDTH;
    initial_pos_y = doodleY ;
    target_pos_x = targetX;
    target_pos_y = targetY;
    decide_move_speed();
    // QT object setting
    setPos(initial_pos_x, initial_pos_y);
    setZValue(STAIR_ITEM_ORDER);
    setPixmap(QPixmap("./dataset/images/bullet.png").scaled(width(), height()));
    //
    
}

bool Bullet::isOutOfScreen()
{
    return 0;// ((y() + height()) >= SCREEN_HEIGHT || y() < 0 || (x() + wisth()) >= SCREEN_WIDTH || x() < 0);
}

int Bullet::width()
{
    return BULLET_WIDTH;
}

int Bullet::height()
{
    return BULLET_HEIGHT;
}

void Bullet::rise(int rise_speed)
{
    setPos(x(), y() - rise_speed);
}
void Bullet::fall(int fall_speed)
{
    setPos(x(), y() + fall_speed);
}
void Bullet::left(int left_speed)
{
    setPos(x() - left_speed, y());
}

void Bullet::right(int right_speed)
{
    setPos(x() + right_speed, y());
}

int Bullet::pos_Left()
{
    return x();
}
int Bullet::pos_Right()
{
    return x() + width();
}
int Bullet::pos_Up()
{
    return y();
}
int Bullet::pos_Down()
{
    return y() + height();
}

void Bullet::bullet_action()
{
    right(moveX);
    fall(moveY);
}

void Bullet::decide_move_speed()
{
    double move_distance = sqrt(pow(initial_pos_x - target_pos_x, 2) + pow((initial_pos_y - target_pos_y), 2));
    double move_unit = move_distance / 40.0;
    moveX = (target_pos_x - initial_pos_x) / move_unit;
    moveY = (target_pos_y - initial_pos_y) / move_unit;

}