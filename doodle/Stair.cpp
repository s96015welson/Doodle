#include "Stair.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <cstdlib>
#include <QPixmap>
#include "Game.h"
#include "Parameter.h"
#include "Health.h"
#include <iostream>

extern Game *game;

Stair::Stair(Score *score, Stair *last_stair, QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
  // decide stair type and initial position
  initial_pos_x = rand() % (SCREEN_WIDTH - STAIR_WIDTH);
  stair_type = decide_stair_type(score, last_stair); // decide stair type
  // QT object setting
  setPos(initial_pos_x, 0);
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
  case stair_moving:
    setPixmap(QPixmap("./dataset/images/stair-blue.png").scaled(STAIR_WIDTH, STAIR_HEIGHT));
    break;
  case stair_broken:
    setPixmap(QPixmap("./dataset/images/stair-broken1.png").scaled(STAIR_WIDTH, STAIR_HEIGHT));
    break;
  case stair_disappear:
    setPixmap(QPixmap("./dataset/images/stair-disapear.png").scaled(STAIR_WIDTH, STAIR_HEIGHT));
  default:
    break;
  }
}

StairType Stair::get_stairtype()
{
  return stair_type;
}
void Stair::change_stair_type(StairType new_type)
{
  stair_type = new_type;
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
void Stair::left(int left_speed)
{
  setPos(x() - left_speed, y());
}

void Stair::right(int right_speed)
{
  setPos(x() + right_speed, y());
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
  return x() + width();
}
int Stair::pos_Up()
{
  return y();
}
int Stair::pos_Down()
{
  return y() + height();
}

StairType Stair::decide_stair_type(Score *score, Stair *last_stair)
{
  static int last_broken_score = 0;

  if (rand() % 100 > STAIR_RATE)
  {
    return stair_basic;
  }
  else
  {
    if (score == nullptr)
      return stair_basic;
    else if (score->getScore() > last_broken_score) // score > 9000
    {
      last_broken_score += 1000;
      return stair_broken;
    }
    else if (score->getScore() > UNLOCK_DISAPPEARING_PLATFORM) // score > 9000
      return (StairType)(rand() % 3);
    else if (score->getScore() > UNLOCK_MOVING_PLATFORM) // score > 6000
      return (StairType)(rand() % 2);
    else
      return stair_basic;
  }
}

void Stair::stair_action()
{
  if (stair_type == stair_moving)
    moving();
  else if ((stair_type == stair_delete) && (isdisappear == true)) // disappear_stair
  {
    fall(SCREEN_HEIGHT);
    ;
  }                                                            // let disappear fall and out of boundary
  else if ((stair_type == stair_delete) && (isbroken == true)) // broken_stair aftertouch and broken
    breaking();                                                // call breaking and do breaking action
}

void Stair::moving()
{
  // moving
  if (moving_direction == LEFT)
    left();
  else if (moving_direction == RIGHT)
    right();
  // out of boundary, change direction
  if ((x() > SCREEN_WIDTH - STAIR_WIDTH) || (x() > initial_pos_x + stair_moving_pixel / 2)) // right->left
  {
    left(STAIR_MOVING_SPEED * 2);
    moving_direction = LEFT;
  }
  else if ((x() < 0) || (x() < initial_pos_x - stair_moving_pixel / 2)) // left->right
  {
    right(STAIR_MOVING_SPEED * 2);
    moving_direction = RIGHT;
  }
}
void Stair::breaking()
{

  if (broken_stage == 0) // change stair type
  {
    // stair_type = stair_disappear;
  }
  else if (broken_stage == 2)
  {
    setPixmap(QPixmap("./dataset/images/stair-broken2.png").scaled(STAIR_BROKEN2_WIDTH, STAIR_BROKEN2_HEIGHT));
    fall(2);
  }
  else if (broken_stage == 3)
  {
    setPixmap(QPixmap("./dataset/images/stair-broken3.png").scaled(STAIR_BROKEN3_WIDTH, STAIR_BROKEN3_HEIGHT));
    fall(2);
  }
  else if (broken_stage == 4)
  {
    setPixmap(QPixmap("./dataset/images/stair-broken4.png").scaled(STAIR_BROKEN4_WIDTH, STAIR_BROKEN4_HEIGHT));
    fall(2);
  }
  else
  {
    fall(stair_fall_speed);
    stair_fall_speed += STAIR_FALLING_ACCELERATION;
  }
  // disappear_stag + 1
  broken_stage++;
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