#ifndef STAIR_H
#define STAIR_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QGraphicsItem>
#include "Player.h"
#include "Score.h"

enum StairType {
    stair_basic, stair_blue, stair_broken, 
    NUM_OF_STAIR_TYPE // This value is intented to keep the number of elements in this enum.
};

class Stair: public QObject,public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Stair(Score * score=nullptr,QGraphicsItem * parent=0);
    void fall(int fall_speed = STAIR_FALLING_SPEED);
    bool isOutOfScreen();
    void takeEffect(Player *player);
    //stair size
    int width();
    int height();
    //stair position
    int pos_Left();
    int pos_Right();
    int pos_Up();
    int pos_Down();

private:
    void initPixmap();
    int decide_type(int);
    bool has_taken_effect = RISE;
    StairType stair_type;
    StairType decide_stair_type(Score*);

    //void normalStairEffect(Player *player,Health *health);
    //void spikeStairEffect(Player *player, Health *health);
  
};

#endif
