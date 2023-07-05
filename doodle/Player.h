#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include "Parameter.h"

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Player(QGraphicsItem *parent = 0, const char * = "dataset/images/doodleUR.png");
    int left_moving_speed = PLAYER_MOVING_SPEED;
    int right_moving_speed = PLAYER_MOVING_SPEED;
    void moveLeft();
    void moveRight();

    int rising_speed = PLAYER_RISING_SPEED;
    void fall(int fall_speed = PLAYER_FALLING_SPEED);
    void rise();
    void jump();
    void fly();

    bool die();

    int width();
    int height();

    int pos_Left();
    int pos_Right();
    int pos_Up();
    int pos_Down();

    int move_action = FALL;
    int move_action_LR = NO_LR;
    int LR_times = 0;
    int UP_times = 0;
};

#endif // PLAYER_H
