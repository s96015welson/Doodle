#ifndef MONSTER_H
#define MONSTER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "Player.h"
#include "Score.h"
#include "Stair.h"

class Monster :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Monster( Stair *stair = nullptr, QGraphicsItem *parent = 0); // constructor


    bool isOutOfScreen();

    //monster moving
    void rise(int rise_speed = 0);
    void fall(int fall_speed = STAIR_FALLING_SPEED);
    void left(int left_speed = STAIR_MOVING_SPEED);
    void right(int right_speed = STAIR_MOVING_SPEED);

    // monster type
    MonsterType get_MonsterType();
    void change_monster_type(MonsterType);

    // item size
    int width();
    int height();
    // item position
    int pos_Left();
    int pos_Right();
    int pos_Up();
    int pos_Down();





private:
    void initPixmap();
    int initial_position_x(Stair *);
    MonsterType decide_monster_type();

    //item position
    int initial_pos_x;
    int initial_pos_y;
    int relative_stair_pos_x = 0;
    MonsterType monster_type;
    //stair under item
    Stair* stair_under_monster;
};

#endif
