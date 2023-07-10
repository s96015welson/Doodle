#ifndef ITEMONSTAIR_H
#define ITEMONSTAIR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "Player.h"
#include "Score.h"
#include "Stair.h"

class ItemOnStair :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    ItemOnStair(Score *score = nullptr, Stair *stair = nullptr, QGraphicsItem *parent = 0); // constructor


    bool isOutOfScreen();

    //item moving
    void rise(int rise_speed = 0);
    void fall(int fall_speed = STAIR_FALLING_SPEED);
    void left(int left_speed = STAIR_MOVING_SPEED);
    void right(int right_speed = STAIR_MOVING_SPEED);

    // item type
    ItemType get_itemtype();
    void change_item_type(ItemType);
    // item size
    int width();
    int height();
    // item position
    int pos_Left();
    int pos_Right();
    int pos_Up();
    int pos_Down();

    //item move
    void item_action();

    //change item_image
    void change_item_image(int new_item = DONT_CARE,int now_item = DONT_CARE);


private:
    void initPixmap();
    int initial_position_x(Stair *);
    ItemType decide_item_type(Score *);

    //item position
    int initial_pos_x;
    int initial_pos_y;
    int relative_stair_pos_x = 0;
    ItemType item_type;
    //stair under item
    Stair* stair_under_item;
};

#endif
