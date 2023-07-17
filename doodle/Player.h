#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include "Parameter.h"

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Player(QGraphicsItem *parent = 0, const char * = "dataset/images/doodleUL.png");
    int left_moving_speed = PLAYER_MOVING_SPEED;
    int right_moving_speed = PLAYER_MOVING_SPEED;
    void moveLeft();
    void moveRight();

    // action
    int fall_speed = 0;
    int rising_speed = PLAYER_RISING_SPEED;
    int jumping_speed = PLAYER_JUMPING_SPEED;
    int flying_speed = PLAYER_FLYING_SPEED;
    void fall(int fall_speed = PLAYER_FALLING_SPEED);
    void rise(int rising_speed = PLAYER_RISING_SPEED);
    void jump(int jumping_speed = PLAYER_JUMPING_SPEED);
    void fly(int flying_speed = PLAYER_FLYING_SPEED);

    bool outOfScreen();

    // position
    int width();
    int height();

    int pos_Left();
    int pos_Right();
    int pos_Up();
    int pos_Down();

    // action parameter
    int move_action = FALL;
    int move_action_LR = NO_LR;
    int LR_times = 0;
    int UP_times = 0;

    // plyer do action
    void player_do_LR_action(int &key);
    void player_do_UPDOWN_action();

    // change_doodle_image
    int doodle_image = DOODLEUL;
    void change_player_image(int new_palyer_image);
    void update_image();

private:
    void set_image(int new_palyer_image);
};

#endif // PLAYER_H
