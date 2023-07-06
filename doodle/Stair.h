#ifndef STAIR_H
#define STAIR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "Player.h"
#include "Score.h"


class Stair : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Stair(Score *score = nullptr, QGraphicsItem *parent = 0); // constructor
    bool isOutOfScreen();
    void takeEffect(Player *player);
    //stair moving
    void fall(int fall_speed = STAIR_FALLING_SPEED);
    void left(int left_speed = STAIR_MOVING_SPEED);
    void right(int right_speed = STAIR_MOVING_SPEED);

    // stair type
    StairType get_stairtype();
    void change_stair_type(StairType);
    // stair size
    int width();
    int height();
    // stair position
    int pos_Left();
    int pos_Right();
    int pos_Up();
    int pos_Down();

    // stair moving/disappearing action
    void stair_action();
    void moving();
    void disappearing();


private:
    void initPixmap();
    int decide_type(int);
    bool has_taken_effect = RISE;
    StairType stair_type;
    StairType decide_stair_type(Score *);


    //moving stair parameter
    int moving_direction = LEFT;
    int stair_moving_pixel = MIN_STAIR_MOVING_PIXEL + rand() % (MAX_STAIR_MOVING_PIXEL - MIN_STAIR_MOVING_PIXEL);
    int initial_pos_x;
    //disappear stair parameter
    int disappear_stage = 0;
    int stair_fall_speed = STAIR_FALLING_SPEED;

    // void normalStairEffect(Player *player,Health *health);
    // void spikeStairEffect(Player *player, Health *health);
};

#endif
