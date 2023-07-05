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
    updatingStairs();
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
                (player->pos_Down() < stair->pos_Down() && player->pos_Down() > stair->pos_Up()))
            {
                // touch stair

                player->move_action = RISE;
                player->UP_times = BASIC_JUMP / PLAYER_RISING_SPEED;
                return stair;
            }
        }
    return nullptr;
}

void Game::updatingStairs()
{
    //   stair5  <-back
    //   stair4
    //   stair3
    //   stair2
    //   stair1  <-front

    // remove stairs that lower than upper bound of screen
    Stair *lowest_stair = (stairs.size() > 0) ? stairs.front() : nullptr;
    if (lowest_stair != nullptr && lowest_stair->isOutOfScreen())
    {
        scene->removeItem(lowest_stair); // remove item from scene
        delete lowest_stair;             // delete stair object
        stairs.pop_front();              // delete stair from queue
    }

    // renew the position of all the stairs
    // for (Stair *stair : stairs)
    //    stair->fall();

    // add new stair
    static int stair_interval = MIN_STAIR_INTERVAL + (rand() % (MAX_STAIR_INTERVAL - MIN_STAIR_INTERVAL));

    Stair *highest_stair = (stairs.size() > 0) ? stairs.back() : nullptr;
    if (highest_stair != nullptr && highest_stair->y() > stair_interval) // add new stair
    {
        // add new stair
        Stair *stair = new Stair(score);
        stairs.push_back(stair);
        scene->addItem(stair);
        // rand stair_interval
        stair_interval = MIN_STAIR_INTERVAL + (rand() % (MAX_STAIR_INTERVAL - MIN_STAIR_INTERVAL));
    }
    else if (highest_stair == nullptr) // initial stairs
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
}

void Game::scrollScreen(int scrollPixel)
{
    // renew the position of all the stairs
    for (Stair *stair : stairs)
        stair->fall(scrollPixel);

    // renew doodle position
    player->fall(scrollPixel);
}
