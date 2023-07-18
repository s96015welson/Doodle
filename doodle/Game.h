#ifndef GAME_H
#define GAME_H

#include <deque>

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QKeyEvent>

#include "Player.h"
#include "Score.h"
#include "Health.h"
#include "Stair.h"
#include "ItemOnStair.h"
#include "Monster.h"
#include "Bullet.h"

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    Game(QWidget *parent = 0);

    // objects of queue
    QGraphicsScene *scene = nullptr;
    Player *player = nullptr;
    Score *score = nullptr;
    Health *health = nullptr;
    std::deque<Stair *> stairs;      // use a queue to store all the stairs; front->high / back->low
    std::deque<ItemOnStair *> items; // use a queue to store all the items; front->high / back->
    std::deque<Bullet *> bullets;    // use a queue to store all the items; front->high / back->
    Monster *monster = nullptr;

    int key = NO_LR;
    int elapsed_frames = 0;

    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    // void keyReleaseEvent(QKeyEvent *event);
    void ShowMsg();


public slots:
    void updating();

protected:
private:
    void createScene();
    void reset();
    void registerUpdatingCallback();
    Stair *getWherePlayerStandingOn(Player *player);
    void touchMonster();
    void updatingStairsandItems();
    void scrollScreen(int);
    void bulletMove();

    int addItem_and_monster(Stair *stair, int);
    //bullet parameter
    bool addBullet =false;
    int targetX;
    int targetY;

    // int time
    int timerID;
    QImage img;
    //
    QGraphicsPixmapItem *start_pic;
};

#endif // GAME_H
