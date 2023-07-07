#include "Game.h"

#include <cstdlib>
#include <iostream>

#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QMessageBox>
#include <ctime>
#include <cstdlib>
#include "Stair.h"
#include "Parameter.h"
#include "ItemOnStair.h"

Game::Game(QWidget *parent)
{
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setMouseTracking(true);
    srand(time(NULL));
    createScene();

    reset();
    std::cout << "reset ready" << std::endl;
    registerUpdatingCallback();

    show();
}

void Game::registerUpdatingCallback()
{
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updating()));
    timer->start(FRAME_DELAY);
}

void Game::createScene()
{
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // make the scene 800x600 instead of infinity by infinity (default)

    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    // ADD BACKGROUND
    QGraphicsPixmapItem *pic = new QGraphicsPixmapItem();
    pic->setPixmap(QPixmap("./dataset/images/background.png"));
    scene->addItem(pic);
    pic->setPos(0, 0);
}

void Game::reset()
{
    key = Qt::Key_No;

    // RESET PLAYER
    if (player)
    {
        scene->removeItem(player);
        delete player;
    }
    player = new Player();
    scene->addItem(player);

    // RESET SCORE
    if (score)
    {
        scene->removeItem(score);
        delete score;
    }
    score = new Score();
    scene->addItem(score);

    // RESET HEALTH
    if (health)
    {
        scene->removeItem(health);
        delete health;
    }
    health = new Health();
    scene->addItem(health);

    // RESET STAIR
    if (!stairs.empty())
    {
        for (Stair *stair : stairs)
        {
            scene->removeItem(stair);
            delete stair;
        }
        stairs.clear();
    }
    // RESET ITEM
    if (!items.empty())
    {
        for (ItemOnStair *item : items)
        {
            scene->removeItem(item);
            delete item;
        }
        items.clear();
    }
}

void Game::keyPressEvent(QKeyEvent *event)
{

    if (event->key() == Qt::Key_Left)
    {
        key = LEFT;
    }
    else if (event->key() == Qt::Key_Right)
    {
        key = RIGHT;
    }
}
/*
void Game::keyReleaseEvent(QKeyEvent * event)
{

    if  (event->key() == Qt::Key_Left && key == Qt::Key_Left && event->isAutoRepeat() == false)
        key = Qt::Key_No;
    else if(event->key() == Qt::Key_Right && key == Qt::Key_Right && event->isAutoRepeat() == false)
        key = Qt::Key_No;
    else if(event->key() == Qt::Key_X && key2 == Qt::Key_X && event->isAutoRepeat() == false)
        key2 = Qt::Key_No;
    else if(event->key() == Qt::Key_Z && key2 == Qt::Key_Z && event->isAutoRepeat() == false)
        key2 = Qt::Key_No;

}

static void ShowMsg(const char *str)
{
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}
*/
// We use the name `updating` in case of method name collision.
void Game::updating()
{

    // paurse?
    if (key == Qt::Key_P)
        return;

    // player move left or right?
    if (key == LEFT)
    {
        key = NO_LR;
        player->move_action_LR = LEFT;
        player->LR_times = LR_DISTANCE / PLAYER_MOVING_SPEED;
    }
    else if (key == RIGHT)
    {
        key = NO_LR;
        player->move_action_LR = RIGHT;
        player->LR_times = LR_DISTANCE / PLAYER_MOVING_SPEED;
    }
    if (player->LR_times > 0)
    {
        player->LR_times--;
        // doddle move left/right
        if (player->move_action_LR == LEFT)
            player->moveLeft();
        else if (player->move_action_LR == RIGHT)
            player->moveRight();
        else if (player->LR_times == 0)
            player->move_action_LR = NO_LR;
    }

    // Get where doodle stand or touch
    Stair *touch_stair;
    if (player->move_action == FALL)
        touch_stair = getWherePlayerStandingOn(player);

    // if ((touch_stair != nullptr) && (touch_stair->get_stairtype() == stair_broken))
    //     touch_stair->change_stair_type(stair_disappear);

    // player rises or falls ?
    if (player->move_action == RISE)
    {
        if (player->UP_times > 0)
        {
            player->UP_times--;
            player->rise();
        }
        else if (player->UP_times == 0)
            player->move_action = FALL;
    }
    else if (player->move_action == FALL)
        player->fall();

    // scroll screen
    int scrollPixel = (SCREEN_HEIGHT / 2) - player->pos_Down();
    if (scrollPixel > 0)
        scrollScreen(scrollPixel);

    // increase score

    if (scrollPixel > 0)
        score->increase(scrollPixel);

    // player die?
    if (player->die() == true)
        std::cout << "die\n";

    // remove, generate and rise stairs
    updatingStairsandItems();
    elapsed_frames++;
}

Stair *Game::getWherePlayerStandingOn(Player *player)
{
    if (player->move_action == FALL)
        for (Stair *stair : stairs)
        {

            // whether touch stair
            if (((player->pos_Left() > stair->pos_Left() && player->pos_Left() < stair->pos_Right()) ||
                 (player->pos_Right() > stair->pos_Left() && player->pos_Right() < stair->pos_Right())) &&
                (player->pos_Down() < stair->pos_Down() && player->pos_Down() > stair->pos_Up()) &&
                (stair->get_stairtype() != stair_delete))
            {
                // touch stair
                if (stair->get_stairtype() == stair_broken)
                {
                    stair->change_stair_type(stair_delete);
                    stair->isbroken = true;
                    // std::cout << stair->get_stairtype() << std::endl;
                    return stair;
                }
                if (stair->get_stairtype() == stair_disappear)
                {
                    stair->change_stair_type(stair_delete);
                    stair->isdisappear = true;
                }
                if (stair->get_stairtype() != stair_broken) // rise if touch stairs excluded stair_broken()
                {
                    player->move_action = RISE;
                    player->UP_times = BASIC_JUMP / PLAYER_RISING_SPEED;
                    return stair;
                }
            }
        }
    return nullptr;
}

void Game::updatingStairsandItems()
{
    //   stair5 <-back
    //   stair4
    //   stair3
    //   stair2
    //   stair1 <-front

    // remove stairs that lower than upper bound of screen
    Stair *lowest_stair = (stairs.size() > 0) ? stairs.front() : nullptr;
    if (lowest_stair != nullptr && lowest_stair->isOutOfScreen())
    {
        scene->removeItem(lowest_stair); // remove item from scene
        delete lowest_stair;             // delete stair object
        stairs.pop_front();              // delete stair from queue
    }

    ItemOnStair *lowest_item = (items.size() > 0) ? items.front() : nullptr;
    if (lowest_item != nullptr && lowest_item->isOutOfScreen())
    {

        scene->removeItem(lowest_item); // remove item from scene
        delete lowest_item;             // delete stair object
        items.pop_front();              // delete stair from queue
    }

    // moving/disappearing stairs action
    for (Stair *stair : stairs)
        stair->stair_action();
    for (ItemOnStair *item : items)
        item->item_action();

    // add new stairn and item
    static int stair_interval = MIN_STAIR_INTERVAL + (rand() % (MAX_STAIR_INTERVAL - MIN_STAIR_INTERVAL));

    Stair *highest_stair = (stairs.size() > 0) ? stairs.back() : nullptr;

    if (highest_stair == nullptr) // initial stairs
    {
        for (int y = SCREEN_HEIGHT - 100; y > 0; y -= MIN_STAIR_INTERVAL + (rand() % (MAX_STAIR_INTERVAL - MIN_STAIR_INTERVAL)))
        {
            // add new stair
            Stair *stair = new Stair(score);
            stairs.push_back(stair);
            scene->addItem(stair);
            // move to pos y
            stair->fall(y);
        }
    }
    else if (highest_stair != nullptr && highest_stair->y() > stair_interval) // add new stair
    {
        // add new stair
        Stair *stair = new Stair(score);
        stairs.push_back(stair);
        scene->addItem(stair);
        // rand stair_interval
        if(stair->get_stairtype()==stair_broken)
            stair_interval = MIN_STAIR_INTERVAL + (rand() % (MAX_STAIR_INTERVAL - MIN_STAIR_INTERVAL))*0.3;
        else
            stair_interval = MIN_STAIR_INTERVAL + (rand() % (MAX_STAIR_INTERVAL - MIN_STAIR_INTERVAL));
        // add item
        addItem(stair);
    }
}

void Game::scrollScreen(int scrollPixel)
{
    // renew the position of all the stairs
    for (Stair *stair : stairs)
        stair->fall(scrollPixel);

    // renew the position of all the items
    for (ItemOnStair *item : items)
        item->fall(scrollPixel);

    // renew doodle position
    player->fall(scrollPixel);
}

void Game::addItem(Stair *stair)
{
    if (score->getScore() > UNLOCK_SPRING_SCORE && rand() % 100 < ITEM_RATE && (stair->get_stairtype() == stair_basic || stair->get_stairtype() == stair_moving))
    {
        // add new item
        ItemOnStair *item = new ItemOnStair(score, stair);

        items.push_back(item);
        scene->addItem(item);
    }
}