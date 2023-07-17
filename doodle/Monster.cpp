#include "Stair.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <cstdlib>
#include <QPixmap>
#include "Game.h"
#include "Parameter.h"
#include "Health.h"
#include "Monster.h"

extern Game *game;

Monster::Monster( Stair *stair, QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{

    //
    stair_under_monster = stair;
    // decide monster type and initial position
    monster_type = decide_monster_type();       // decide item type
    initial_pos_x = initial_position_x(stair); // decide item pos x
    initial_pos_y = stair->pos_Up() - height();
    // QT object setting
    setPos(initial_pos_x, initial_pos_y);
    setZValue(STAIR_ITEM_ORDER);
    initPixmap();
}

MonsterType Monster::decide_monster_type()
{
    return (MonsterType)(rand() % 4);
}
void Monster::change_monster_type(MonsterType newtype)
{
    monster_type = newtype;
}

int Monster::initial_position_x(Stair *stair)
{
    relative_stair_pos_x = STAIR_WIDTH*0.5 - width()*0.5;
    return stair->x() + relative_stair_pos_x;
}

void Monster::initPixmap()
{
    switch (monster_type)
    {
    case type1:
        setPixmap(QPixmap("./dataset/images/monster1.png").scaled(width(), height()));
        break;
    case type2:
        setPixmap(QPixmap("./dataset/images/monster2.png").scaled(width(), height()));
        break;
    case type3:
        setPixmap(QPixmap("./dataset/images/monster3.png").scaled(width(), height()));
        break;
    case type4:
        setPixmap(QPixmap("./dataset/images/monster4.png").scaled(width(), height()));
    default:
        break;
    }
}
bool Monster::isOutOfScreen()
{
    return (y() + height()*0.5) >= SCREEN_HEIGHT;
}

MonsterType Monster::get_MonsterType()
{

    return monster_type;
}


int Monster::width()
{
    switch (monster_type)
    {
    case type1:
        return MONSTER_TYPE1_WIDTH;
    case type2:
        return MONSTER_TYPE2_WIDTH;
    case type3:
        return MONSTER_TYPE3_WIDTH;
    case type4:
        return MONSTER_TYPE4_WIDTH;
    default:
        return 0;
    }
    return 0;
}

int Monster::height()
{
    switch (monster_type)
    {
    case type1:
        return MONSTER_TYPE1_HEIGHT;
    case type2:
        return MONSTER_TYPE2_HEIGHT;
    case type3:
        return MONSTER_TYPE3_HEIGHT;
    case type4:
        return MONSTER_TYPE4_HEIGHT;
    default:
        return 0;
    }
    return 0;
}

void Monster::rise(int rise_speed)
{
    setPos(x(), y() - rise_speed);
}
void Monster::fall(int fall_speed)
{
    setPos(x(), y() + fall_speed);
}
void Monster::left(int left_speed)
{
    setPos(x() - left_speed, y());
}

void Monster::right(int right_speed)
{
    setPos(x() + right_speed, y());
}

int Monster::pos_Left()
{
    return x();
}
int Monster::pos_Right()
{
    return x() + width();
}
int Monster::pos_Up()
{
    return y();
}
int Monster::pos_Down()
{
    return y() + height();
}
