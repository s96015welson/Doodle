#include "Player.h"
#include <iostream>
#include <QGraphicsScene>
#include <typeinfo>
#include "Stair.h"
#include "Game.h"
#include "Parameter.h"
#include <QDebug>
#include <QPainter>
#include <QPixmap>
extern Game *game; // there is an external global object called game

Player::Player(QGraphicsItem *parent, const char *img) : QObject(), QGraphicsPixmapItem(parent)
{
    setPos(PLAYER_START_POSITION_X, PLAYER_START_POSITION_Y); // generalize to always be in the middle top of screen
    setZValue(PLAYER_ITEM_ORDER);
    setPixmap(QPixmap(img).scaled(DOODLE_WIDTH, DOODLE_HEIGHT));
}

void Player::moveLeft()
{
    if (x() + DOODLE_WIDTH - left_moving_speed > 0)
        setPos(x() - left_moving_speed, y());
    else
        setPos(x() - left_moving_speed + SCREEN_WIDTH, y());
}

void Player::moveRight()
{
    if (x() + right_moving_speed < SCREEN_WIDTH)
        setPos(x() + right_moving_speed, y());
    else
        setPos(x() + right_moving_speed - SCREEN_WIDTH, y());
}

void Player::fall(int fall_speed)
{
    setPos(x(), y() + fall_speed);
}

void Player::rise(int rising_speed)
{
    setPos(x(), y() - rising_speed);
}

void Player::jump(int jumping_speed)
{
    setPos(x(), y() - jumping_speed);
}
void Player::fly(int flying_speed)
{
    setPos(x(), y() - flying_speed);
}

bool Player::die()
{
    return (y() + height() * 0.5) >= SCREEN_HEIGHT;
}

int Player::width()
{
    return DOODLE_WIDTH;
}

int Player::height()
{
    return DOODLE_HEIGHT;
}

int Player::pos_Left()
{
    return x();
}
int Player::pos_Right()
{
    return x() + DOODLE_WIDTH;
}
int Player::pos_Up()
{
    return y();
}
int Player::pos_Down()
{
    return y() + DOODLE_HEIGHT;
}

void Player::player_do_LR_action(int &key)
{
    if (key == LEFT)
    {
        change_player_image(DOODLEL);
        key = NO_LR;
        move_action_LR = LEFT;
        LR_times = LR_DISTANCE / PLAYER_MOVING_SPEED;
    }
    else if (key == RIGHT)
    {
        change_player_image(DOODLER);
        key = NO_LR;
        move_action_LR = RIGHT;
        LR_times = LR_DISTANCE / PLAYER_MOVING_SPEED;
    }
    if (LR_times > 0)
    {
        LR_times--;
        // doddle move left/right
        if (move_action_LR == LEFT)
            moveLeft();
        else if (move_action_LR == RIGHT)
            moveRight();
        else if (LR_times == 0)
            move_action_LR = NO_LR;
    }
}

void Player::player_do_UPDOWN_action()
{

    if (UP_times > 0)
    {
        UP_times--;
        if (move_action == RISE)
        {
            rise(rising_speed);
            rising_speed -= PLAYER_ACCELRATION;
        }

        else if (move_action == JUMP)
        {
            jump(jumping_speed);
            jumping_speed -= PLAYER_ACCELRATION;
            // std::cout << jumping_speed << std::endl;
        }

        else if (move_action == FLY)
        {
            fly(flying_speed);
            //std::cout << flying_speed << std::endl;
            if (UP_times < 10 && flying_speed >= 0)
                flying_speed *= PLAYER_FLY_SLOW_DOWN_RATE;
        }
    }

    if (UP_times == 0 && move_action != FALL)
    {
        move_action = FALL;
        change_player_image(DOODLE_FALL);
        // reset move speed
        fall_speed = 0;
        rising_speed = PLAYER_RISING_SPEED;
        jumping_speed = PLAYER_JUMPING_SPEED;
        flying_speed = PLAYER_FLYING_SPEED;
    }

    else if (move_action == FALL)
    {
        fall(fall_speed);
        if (fall_speed < 26)
            fall_speed += PLAYER_ACCELRATION;
        // std::cout <<"fall speed"<< fall_speed << std::endl;
    }
}

void Player::change_player_image(int new_image_code)
{
    if (new_image_code == DOODLEL)
    {
        if (doodle_image == DOODLEUR)
            set_image(DOODLEUL);

        else if (doodle_image == DOODLER)
            set_image(DOODLEL);
    }
    else if (new_image_code == DOODLER)
    {
        if (doodle_image == DOODLEUL)
            set_image(DOODLEUR);

        else if (doodle_image == DOODLEL)
            set_image(DOODLER);
    }
    else if (new_image_code == DOODLE_FALL)
    {
        if (doodle_image == DOODLER)
            set_image(DOODLEUR);

        else if (doodle_image == DOODLEL)
            set_image(DOODLEUL);
    }
    else if (new_image_code == DOODLE_RISE)
    {
        if (doodle_image == DOODLEUR)
            set_image(DOODLER);

        else if (doodle_image == DOODLEUL)
            set_image(DOODLEL);
    }
}
void Player::set_image(int new_player_image)
{
    doodle_image = new_player_image;
    switch (doodle_image)
    {
    case DOODLEL:
        setPixmap(QPixmap("./dataset/images/doodleL.png").scaled(width(), height()));
        break;
    case DOODLER:
        setPixmap(QPixmap("./dataset/images/doodleR.png").scaled(width(), height()));
        break;
    case DOODLEUL:
        setPixmap(QPixmap("./dataset/images/doodleUL.png").scaled(width(), height()));
        break;
    case DOODLEUR:
        setPixmap(QPixmap("./dataset/images/doodleUR.png").scaled(width(), height()));
        break;
    default:
        break;
    }
}