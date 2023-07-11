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

void Game::updating()
{

    // paurse?
    if (key == Qt::Key_P)
        return;

    // player move left or right?
    player->player_do_LR_action(key);

    // Get where doodle stand or touch
    Stair *touch_stair;
    if (player->move_action == FALL)
        touch_stair = getWherePlayerStandingOn(player);

    // player rises or falls ?
    player->player_do_UPDOWN_action();

    // scroll screen
    int scrollPixel = (SCREEN_HEIGHT / 2) - player->pos_Down();
    if (scrollPixel > 0)
        scrollScreen(scrollPixel);

    // increase score
    if (scrollPixel > 0)
        score->increase(scrollPixel);

    // player die?
    if (player->die() == true)
    {
        // std::cout << "die\n";
    }

    // remove, generate and rise stairs
    updatingStairsandItems();

    // update doodle image
    player->update_image();
    // elapsed_frames++;
    //
}

Stair *Game::getWherePlayerStandingOn(Player *player)
{
    if (player->move_action == FALL)
    {
        // touch item
        for (ItemOnStair *item : items)
        {
            if (((player->pos_Left() > item->pos_Left() && player->pos_Left() < item->pos_Right()) ||
                 (player->pos_Left() + 40 > item->pos_Left() && player->pos_Left() + 40 < item->pos_Right()) ||
                 (player->pos_Right() > item->pos_Left() && player->pos_Right() < item->pos_Right())) &&
                (player->pos_Down() < item->pos_Down() && player->pos_Down() > item->pos_Up()) &&
                (item->get_itemtype() != item_delete))
            {
                if (item->get_itemtype() == spring) // sdpring
                {
                    player->move_action = JUMP;
                    player->UP_times = PLAYER_SPRING_JUMPING_TIMES;
                    player->jumping_speed = PLAYER_SPRING_JUMPING_SPEED;
                    item->change_item_image(SPRING2);
                }

                else if (item->get_itemtype() == trampoline) // trampoline
                {
                    player->move_action = JUMP;
                    player->UP_times = PLAYER_TRAMPOLINE_JUMPING_TIMES;
                    player->jumping_speed = PLAYER_TRAMPOLINE_JUMPING_SPEED;
                    item->change_item_image(TRAMPOLINE3);
                }

                else if (item->get_itemtype() == propeller_hat) // propeller hat
                {
                    player->move_action = FLY;
                    player->UP_times = PLAYER_PROPELLER_HAT_FLYING_TIMES;
                    player->flying_speed = PLAYER_PROPELLER_HAT_FLYING__SPEED;
                    item->fall(SCREEN_HEIGHT);
                    player->change_player_image(DOODLEH1);
                }

                else if (item->get_itemtype() == jet_pack) // jet pack
                {
                    player->move_action = FLY;
                    player->UP_times = PLAYER_JETPACK_FLYING_TIMES;
                    player->flying_speed = PLAYER_JETPACK_FLYING__SPEED;
                    item->fall(SCREEN_HEIGHT);
                    player->change_player_image(DOODLEJ1);
                }

                item->change_item_type(item_delete);
                // item->fall(SCREEN_HEIGHT);

                return nullptr;
            }
        }

        // touch stair
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
                    player->UP_times = PLAYER_RISING_TIMES;
                    player->change_player_image(DOODLE_RISE);
                    return stair;
                }
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
        Stair *stair = new Stair(score, highest_stair);
        stairs.push_back(stair);
        scene->addItem(stair);
        // rand stair_interval
        if (stair->get_stairtype() == stair_broken)
            stair_interval = MIN_STAIR_INTERVAL + (rand() % (MAX_STAIR_INTERVAL - MIN_STAIR_INTERVAL)) * 0.3;
        else
            stair_interval = MIN_STAIR_INTERVAL + (rand() % (MAX_STAIR_INTERVAL - MIN_STAIR_INTERVAL));
        // add item
        addItem(stair);
        //
        if (player->move_action == FLY)
            stair_interval = MIN_STAIR_INTERVAL + (stair_interval - MIN_STAIR_INTERVAL) * 0.8;
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