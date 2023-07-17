#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "Player.h"
#include "Score.h"

class Bullet : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Bullet(int doodleX = 0, int doodleY = 0, int targetX = 0, int targetY = 0, QGraphicsItem *parent = 0); // constructor

    bool isOutOfScreen();

    // Bullet moving
    void rise(int rise_speed = 0);
    void fall(int fall_speed = 0);
    void left(int left_speed = 0);
    void right(int right_speed = 0);


    // item size
    int width();
    int height();
    // item position
    int pos_Left();
    int pos_Right();
    int pos_Up();
    int pos_Down();
    //
    int moveX;
    int moveY;

    void bullet_action();


private:
    // item position
    int initial_pos_x;
    int initial_pos_y;
    int target_pos_x;
    int target_pos_y;
    void decide_move_speed();
};

#endif
