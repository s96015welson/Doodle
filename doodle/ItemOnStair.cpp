#include "Stair.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <cstdlib>
#include <QPixmap>
#include "Game.h"
#include "Parameter.h"
#include "Health.h"
#include "ItemOnStair.h"

extern Game *game;

ItemOnStair::ItemOnStair(Score *score, Stair *stair, QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{

    //
    stair_under_item = stair;
    // decide item type and initial position
    item_type = decide_item_type(score);       // decide item type
    initial_pos_x = initial_position_x(stair); // decide item pos x
    initial_pos_y = stair->pos_Up() - height();
    // QT object setting
    setPos(initial_pos_x, initial_pos_y);
    setZValue(STAIR_ITEM_ORDER);
    initPixmap();
}

ItemType ItemOnStair::decide_item_type(Score *score)
{
    if (score == nullptr)
        return item_delete;
    else if (score->getScore() > UNLOCK_JET_PACK_SCORE) // score > 9000
        return (ItemType)(rand() % 4);
    else if (score->getScore() > UNLOCK_PROPELLER_HAT_SCORE) // score > 6000
        return (ItemType)(rand() % 3);
    else if (score->getScore() > UNLOCK_TRAMPOLINE_SCORE) // score > 6000
        return (ItemType)(rand() % 2);
    else if (score->getScore() > UNLOCK_SPRING_SCORE) // score > 6000
        return (ItemType)(rand() % 1);
    else // NUM_OF_STAIR_TYPE
        return item_delete;
}

void ItemOnStair::item_action()
{
    if (stair_under_item->get_stairtype() == stair_moving)
    {
        setPos(stair_under_item->pos_Left() + relative_stair_pos_x, y());
    }
}

int ItemOnStair::initial_position_x(Stair *stair)
{
    relative_stair_pos_x = (rand() % (STAIR_WIDTH - width()));
    return stair->x() + relative_stair_pos_x;
}

void ItemOnStair::initPixmap()
{
    switch (item_type)
    {
    case spring:
        setPixmap(QPixmap("./dataset/images/item-spring1.png").scaled(width(), height()));
        break;
    case trampoline:
        setPixmap(QPixmap("./dataset/images/item-trampoline1.png").scaled(width(), height()));
        break;
    case propeller_hat:
        setPixmap(QPixmap("./dataset/images/item-propeller-hat.png").scaled(width(), height()));
        break;
    case jet_pack:
        setPixmap(QPixmap("./dataset/images/item-jetpack.png").scaled(width(), height()));
    default:
        break;
    }
}
bool ItemOnStair::isOutOfScreen()
{
    return (y() + height()) >= SCREEN_HEIGHT;
}

ItemType ItemOnStair::get_itemtype()
{
    return item_type;
}
void ItemOnStair::change_item_type(ItemType new_type)
{
    item_type = new_type;
}

int ItemOnStair::width()
{
    switch (item_type)
    {
    case spring:
        return SPRING_WIDTH;
    case trampoline:
        return TRAMPOLINE_WIDTH;
    case propeller_hat:
        return PROPELLER_HAT_WIDTH;
    case jet_pack:
        return JET_PACK_WIDTH;
    default:
        return 0;
    }
    return 0;
}

int ItemOnStair::height()
{
    switch (item_type)
    {
    case spring:
        return SPRING_HEIGHT;
    case trampoline:
        return TRAMPOLINE_HEIGHT;
    case propeller_hat:
        return PROPELLER_HAT_HEIGHT;
    case jet_pack:
        return JET_PACK_HEIGHT;
    default:
        return 0;
    }
    return 0;
}

void ItemOnStair::fall(int fall_speed)
{
    setPos(x(), y() + fall_speed);
}
void ItemOnStair::left(int left_speed)
{
    setPos(x() - left_speed, y());
}

void ItemOnStair::right(int right_speed)
{
    setPos(x() + right_speed, y());
}

int ItemOnStair::pos_Left()
{
    return x();
}
int ItemOnStair::pos_Right()
{
    return x() + width();
}
int ItemOnStair::pos_Up()
{
    return y();
}
int ItemOnStair::pos_Down()
{
    return y() + height();
}
