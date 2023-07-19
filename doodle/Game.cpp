#include "Game.h"

#include <cstdlib>
#include <iostream>

#include <QTimer>
#include <QGraphicsTextItem>
#include <QFont>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QSound>
#include <QMessageBox>
#include <ctime>
#include <cstdlib>
#include "Stair.h"
#include "Parameter.h"
#include "ItemOnStair.h"
#include "Monster.h"
#include "Bullet.h"
#include "unistd.h"
#include <string>

Game::Game(QWidget *parent)
{
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setMouseTracking(true);
    srand(time(NULL));
    createScene();
    // start
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
void Game::ShowMsg()
{
    QMessageBox msgBox;
    // QString final_score = ;

    msgBox.setStyleSheet("QLabel{"
                         "min-width:180px;"
                         "min-height:100px;"
                         "font-size:16px;"
                         "}");
    QString qStr1 = "Game Over!\nFinal Score:";
    QString qStr2 = QString::number(score->getScore());
    msgBox.setText(qStr1 + qStr2);
    msgBox.exec();
}

void Game::createScene()
{
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // make the newly created scene the scene to visualize (since Game is a QGraphicsView Widget,
    // it can be used to visualize scenes)
    setScene(scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    // ADD BACKGROUND


    QGraphicsPixmapItem *pic = new QGraphicsPixmapItem();
    pic->setPixmap(QPixmap("./dataset/images/background.png").scaled(540, 960));
    scene->addItem(pic);
    pic->setPos(0, 0);

    // ADD start

    start_pic = new QGraphicsPixmapItem();
    start_pic->setPixmap(QPixmap("./dataset/images/reference-start-interface.png").scaled(540, 960));
    scene->addItem(start_pic);
    start_pic->setPos(0, 0);
    start_pic->setZValue(START_SCENE);
}

void Game::reset()
{
    // setting renew
    addBullet = false;
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
    // RESET ITEM
    if (!bullets.empty())
    {
        for (Bullet *bullet : bullets)
        {
            scene->removeItem(bullet);
            delete bullet;
        }
        bullets.clear();
    }

    // RESET MONSTER
    if (monster)
    {
        scene->removeItem(monster);
        monster = nullptr;
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
void Game::mousePressEvent(QMouseEvent *event)
{

    targetX = event->x();
    targetY = event->y();

    if (start_pic != nullptr && (targetX > 90 && targetX < 270 && targetY > 250 && targetY < 320)) // start
    {
        QSound::play("./dataset/sounds/jump.wav");
        scene->removeItem(start_pic);
        start_pic = nullptr;
    }
    else if (start_pic == nullptr) // bullet
        addBullet = true;
}

void Game::updating()
{

    // Bedore start
    if (start_pic != nullptr)
        return;
    // player die?
    if (player->outOfScreen() == true || health->getHealth() <= 0)
    {
        if (player->outOfScreen() == true)
            QSound::play("./dataset/sounds/falldown.wav");
        else
            QSound::play("./dataset/sounds/jumponmonster.wav");

        ShowMsg();
        reset();
        return;
    }

    // player move left or right?
    player->player_do_LR_action(key);

    // touch monster?
    touchMonster();

    // Get where doodle stand or touch
    Stair *touch_stair;
    if (player->move_action == FALL || player->move_action == RISE)
        touch_stair = getWherePlayerStandingOn(player);

    // player rises or falls ?
    player->player_do_UPDOWN_action();

    // add bullet

    if (addBullet)
    {
        QSound::play("./dataset/sounds/shoot.wav");

        addBullet = false;
        Bullet *bullet = new Bullet(player->x(), player->y(), targetX, targetY);
        bullets.push_back(bullet);
        scene->addItem(bullet);
    }

    // scroll screen
    int scrollPixel = (SCREEN_HEIGHT / 2) - player->pos_Down();
    if (scrollPixel > 0)
        scrollScreen(scrollPixel);

    // increase score
    if (scrollPixel > 0)
        score->increase(scrollPixel);

    // remove, generate and rise stairs
    updatingStairsandItems();

    // update doodle image
    player->update_image();
    // elapsed_frames++;
    //
}

Stair *Game::getWherePlayerStandingOn(Player *player)
{
    if (player->move_action == FALL || player->move_action == RISE || player->move_action == JUMP)
    {
        // touch item
        for (ItemOnStair *item : items)

        {
            // std::cout << "****" << item->pos_Left() << " " << item->pos_Up() <<" "<<item->get_itemtype()<< "\n";
            // std::cout << "*//*" << player->pos_Left() << " " << player->pos_Up() << "\n";
            if (((player->pos_Left() > item->pos_Left() && player->pos_Left() < item->pos_Right()) ||
                 (player->pos_Left() + 40 > item->pos_Left() && player->pos_Left() + 40 < item->pos_Right()) ||
                 (player->pos_Right() > item->pos_Left() && player->pos_Right() < item->pos_Right())) &&
                ((player->pos_Down() < item->pos_Down() && player->pos_Down() > item->pos_Up()) ||
                 (player->pos_Up() + 60 < item->pos_Down() && player->pos_Up() + 60 > item->pos_Up()) ||
                 (player->pos_Up() + 40 < item->pos_Down() && player->pos_Up() + 40 > item->pos_Up()) ||
                 (player->pos_Up() + 20 < item->pos_Down() && player->pos_Up() + 20 > item->pos_Up()) ||
                 (player->pos_Up() < item->pos_Down() && player->pos_Up() > item->pos_Up())) &&
                (item->get_itemtype() != item_delete) && (player->move_action == FALL))
            {
                // std::cout << player->x() << " " << player->y() << "/" << item->pos_Left() << " " << item->pos_Up() << "\n";
                // std::cout << player->x() + 80 << " " << player->y() + 80 << "/" << item->pos_Right() << " " << item->pos_Down() << "\n";
                // std::cout << "-----------------\n";
                if ((player->move_action == FALL) &&
                    ((player->pos_Down() < item->pos_Down() && player->pos_Down() > item->pos_Up()) ||
                     (player->pos_Down() - 20 < item->pos_Down() && player->pos_Down() - 20 > item->pos_Up())))
                {
                    if (item->get_itemtype() == spring) // sdpring
                    {
                        player->move_action = JUMP;
                        player->UP_times = PLAYER_SPRING_JUMPING_TIMES;
                        player->jumping_speed = PLAYER_SPRING_JUMPING_SPEED;
                        item->change_item_image(SPRING2);
                        item->change_item_type(item_delete);
                        player->change_player_image(DOODLE_RISE);
                        QSound::play("./dataset/sounds/spring.wav");
                    }

                    else if (item->get_itemtype() == trampoline) // trampoline
                    {
                        player->move_action = JUMP;
                        player->UP_times = PLAYER_TRAMPOLINE_JUMPING_TIMES;
                        player->jumping_speed = PLAYER_TRAMPOLINE_JUMPING_SPEED;
                        item->change_item_image(TRAMPOLINE3);
                        item->change_item_type(item_delete);
                        player->change_player_image(DOODLE_RISE);
                        QSound::play("./dataset/sounds/trampoline.wav");
                    }
                }

                if (item->get_itemtype() == propeller_hat) // propeller hat
                {
                    player->move_action = FLY;
                    player->UP_times = PLAYER_PROPELLER_HAT_FLYING_TIMES;
                    player->flying_speed = PLAYER_PROPELLER_HAT_FLYING__SPEED;
                    item->fall(SCREEN_HEIGHT);
                    player->change_player_image(DOODLEH1);
                    item->change_item_type(item_delete);
                    QSound::play("./dataset/sounds/propeller.wav");
                }

                else if (item->get_itemtype() == jet_pack) // jet pack
                {
                    player->move_action = FLY;
                    player->UP_times = PLAYER_JETPACK_FLYING_TIMES;
                    player->flying_speed = PLAYER_JETPACK_FLYING__SPEED;
                    item->fall(SCREEN_HEIGHT);
                    player->change_player_image(DOODLEJ1);
                    item->change_item_type(item_delete);
                    QSound::play("./dataset/sounds/rocket.wav");
                }

                return nullptr;
            }
        }
    }
    if (player->move_action == FALL)
    {
        // touch stair
        for (Stair *stair : stairs)
        {

            // whether touch stair
            if (((player->pos_Left() > stair->pos_Left() && player->pos_Left() < stair->pos_Right()) ||
                 (player->pos_Right() > stair->pos_Left() && player->pos_Right() < stair->pos_Right())) &&
                (player->pos_Down() < stair->pos_Down() && player->pos_Down() > stair->pos_Up()) &&
                (stair->get_stairtype() != stair_delete))
            {
                if ((player->LR == LEFT && player->pos_Left() + 20 < stair->pos_Right()) || (player->LR == RIGHT && player->pos_Right() - 20 > stair->pos_Left()))
                {
                    // touch stair
                    if (stair->get_stairtype() == stair_broken)
                    {
                        stair->change_stair_type(stair_delete);
                        stair->isbroken = true;
                        QSound::play("./dataset/sounds/break.wav");
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
                        QSound::play("./dataset/sounds/jump.wav");
                        return stair;
                    }
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

    Bullet *first_bullet = (bullets.size() > 0) ? bullets.front() : nullptr;
    if (first_bullet != nullptr && first_bullet->isOutOfScreen())
    {

        scene->removeItem(first_bullet); // remove item from scene
        delete first_bullet;             // delete stair object
        bullets.pop_front();             // delete stair from queue
    }

    if (monster != nullptr && monster->isOutOfScreen())
    {
        scene->removeItem(monster);
        monster = nullptr;
    }

    // moving/disappearing stairs action
    for (Stair *stair : stairs) // do stair actions
        stair->stair_action();
    for (ItemOnStair *item : items) // do item actions
        item->item_action();

    for (Bullet *bullet : bullets) // do bullet actions
        bullet->bullet_action();

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
        // add item or monster
        stair_interval = addItem_and_monster(stair, stair_interval);
        //
        if (player->move_action == FLY)
            stair_interval = MIN_STAIR_INTERVAL + (stair_interval - MIN_STAIR_INTERVAL) * 0.8;
    }
}

void Game::scrollScreen(int scrollPixel)
{
    // scroll  all the stairs
    for (Stair *stair : stairs)
        stair->fall(scrollPixel);

    // scroll all the items
    for (ItemOnStair *item : items)
        item->fall(scrollPixel);

    // scroll doodle position
    player->fall(scrollPixel);
    // scroll monster position
    if (monster)
        monster->fall(scrollPixel);

    // scroll bullet position
    for (Bullet *bullet : bullets)
        bullet->fall(scrollPixel);
}

int Game::addItem_and_monster(Stair *stair, int stair_interval)
{
    if (score->getScore() > UNLOCK_SPRING_SCORE && items.size() < 2 && rand() % 100 < ITEM_RATE && (stair->get_stairtype() == stair_basic || stair->get_stairtype() == stair_moving))
    {
        // add new item
        ItemOnStair *item = new ItemOnStair(score, stair);


        if (stair_interval < item->height())
            stair_interval = item->height() * 1.1;
    }
    else if (score->getScore() > UNLOCK_MONSTER_SCORE && rand() % 100 < MONSTER_RATE && (stair->get_stairtype() == stair_basic))
    {
        if (monster == nullptr)
        {
            monster = new Monster(stair);
            scene->addItem(monster);
            if (stair_interval < monster->height() * 1.1 + STAIR_HEIGHT)
                stair_interval = monster->height() * 1.1 + STAIR_HEIGHT;
        }
    }

    return stair_interval;
}

void Game::touchMonster()
{
    static bool lose_HP_cool_down = false;
    if (monster)
    {
        // bullet touch monster and kill monster
        for (Bullet *bullet : bullets)
        {
            if ((bullet->pos_Left() + 10 > monster->pos_Left() && bullet->pos_Left() + 10 < monster->pos_Right()) &&
                (bullet->pos_Up() + 10 > monster->pos_Up() && bullet->pos_Up() + 10 < monster->pos_Down()) &&
                (monster->get_MonsterType() != monster_delete))
            {
                // kill monster
                monster->change_monster_type(monster_delete);
                monster->fall(SCREEN_HEIGHT * 2);

                // remove bullet
            }
        }
        // doodle jump and kill monster
        if (player->move_action == FALL)
        {
            if (((player->pos_Left() > monster->pos_Left() && player->pos_Left() < monster->pos_Right()) ||
                 (player->pos_Left() + 40 > monster->pos_Left() && player->pos_Left() + 40 < monster->pos_Right()) ||
                 (player->pos_Right() > monster->pos_Left() && player->pos_Right() < monster->pos_Right())) &&
                (player->pos_Down() < monster->pos_Up() + 30 && player->pos_Down() > monster->pos_Up()) &&
                (monster->get_MonsterType() != monster_delete))
            {
                QSound::play("./dataset/sounds/jumponmonster.wav");
                // doodle rise
                player->move_action = RISE;
                player->UP_times = PLAYER_RISING_TIMES;
                player->change_player_image(DOODLE_RISE);
                // monster
                monster->change_monster_type(monster_delete);
                monster->fall(SCREEN_HEIGHT);
                return;
            }
        }
        // doodle touch monster and minus HP
        if (1)
        {
            const int tolerance = 20;
            if (((player->pos_Left() > monster->pos_Left() + tolerance && player->pos_Left() < monster->pos_Right() - tolerance) ||
                 (player->pos_Left() + 40 > monster->pos_Left() + tolerance && player->pos_Left() + 40 < monster->pos_Right() - tolerance) ||
                 (player->pos_Right() > monster->pos_Left() + tolerance && player->pos_Right() < monster->pos_Right() - tolerance)) &&
                ((player->pos_Up() > monster->pos_Up() + tolerance && player->pos_Up() < monster->pos_Down() - tolerance) ||
                 (player->pos_Up() + 40 > monster->pos_Up() + tolerance && player->pos_Up() + 40 < monster->pos_Down() - tolerance) ||
                 (player->pos_Down() > monster->pos_Up() + tolerance && player->pos_Down() < monster->pos_Down() - tolerance)) &&
                (monster->get_MonsterType() != monster_delete))
            {
                if (lose_HP_cool_down == false)
                {
                    health->decrease(1);
                    lose_HP_cool_down = true;
                }
            }
            else
                lose_HP_cool_down = false;
        }
    }
}