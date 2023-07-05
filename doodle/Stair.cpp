#include "Stair.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <cstdlib>
#include <QPixmap>
#include "Game.h"
#include "Parameter.h"
#include "Health.h"

extern Game *game;

Stair::Stair(Score *score, QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
  int random_x = rand() % (SCREEN_WIDTH - STAIR_WIDTH);

  stair_type = decide_stair_type(score); // decide stair type
  setPos(random_x, 0);
  setZValue(STAIR_ITEM_ORDER);
  initPixmap();
}

void Stair::initPixmap()
{
  switch (stair_type)
  {
  case stair_basic:
    setPixmap(QPixmap("./dataset/images/stair-basic.png").scaled(STAIR_WIDTH, STAIR_HEIGHT));
    break;
  case stair_blue:
    setPixmap(QPixmap("./dataset/images/stair-blue.png").scaled(STAIR_WIDTH, STAIR_HEIGHT));
    break;
  case stair_broken:
    setPixmap(QPixmap("./dataset/images/stair-broken1.png").scaled(STAIR_WIDTH, STAIR_HEIGHT));
    break;
  case NUM_OF_STAIR_TYPE:
    break;
  }
}

int Stair::width()
{
  return STAIR_WIDTH;
}

int Stair::height()
{
  return STAIR_HEIGHT;
}

void Stair::fall(int fall_speed)
{
  setPos(x(), y() + fall_speed);
}

bool Stair::isOutOfScreen()
{
  return (y() + height()) >= SCREEN_HEIGHT;
}

int Stair::pos_Left()
{
  return x();
}
int Stair::pos_Right()
{
  return x() + STAIR_WIDTH;
}
int Stair::pos_Up()
{
  return y();
}
int Stair::pos_Down()
{
  return y() + STAIR_HEIGHT;
}

StairType Stair::decide_stair_type(Score *score)
{
  if (score == nullptr)
    return (StairType)0;
  else if (score->getScore() > UNLOCK_DISAPPEARING_PLATFORM) // score > 9000
    return (StairType)(rand() % 3);
  else if (score->getScore() > UNLOCK_MOVING_PLATFORM) // score > 6000
    return (StairType)(rand() % 2);
  else //
    return (StairType)0;
}

/*
void Stair::takeEffect(Player *player) {
    switch(stair_type) {
      case spike_stair:
        spikeStairEffect(player,health); break;
      case normal_stair:
        normalStairEffect(player,health); break;
      case left_roll_stair:
        leftRollStairEffect(player,health); break;
      case right_roll_stair:
        rightRollStairEffect(player,health); break;
    }
    has_taken_effect = true;
}


void Stair::normalStairEffect(Player *player,Health *health) {
  if (!has_taken_effect && game->health->getHealth() < DEFAULT_HEALTH)
    game->health->increase(1);
}

void Stair::spikeStairEffect(Player *player,Health *health) {
    if (!has_taken_effect)
        health->decrease(5); // If the health is negative, updating() in Game.cpp will regard it as death
}

void Stair::leftRollStairEffect(Player *player,Health *health) {
  game->player->setPos(game->player->x() - LEFT_ROLL_STAIR_SPEED,game->player->y());
  if (!has_taken_effect && game->health->getHealth() < DEFAULT_HEALTH)
    game->health->increase(1);
}

void Stair::rightRollStairEffect(Player *player,Health *health) {
  game->player->setPos(game->player->x() + RIGHT_ROLL_STAIR_SPEED,game->player->y());
  if (!has_taken_effect && game->health->getHealth() < DEFAULT_HEALTH)
    game->health->increase(1);
}
*/