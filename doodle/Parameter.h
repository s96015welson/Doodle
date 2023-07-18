#ifndef PARAMETER_H
#define PARAMETER_H

#define FRAME_DELAY 30

// basic image size
#define SCREEN_WIDTH 540
#define SCREEN_HEIGHT 960

#define DOODLE_WIDTH 80
#define DOODLE_HEIGHT 80

#define STAIR_WIDTH 120
#define STAIR_HEIGHT 30

#define SPRING_WIDTH 40
#define SPRING_HEIGHT 25

#define TRAMPOLINE_WIDTH 72
#define TRAMPOLINE_HEIGHT 36

#define PROPELLER_HAT_WIDTH 64
#define PROPELLER_HAT_HEIGHT 40

#define JET_PACK_WIDTH 50
#define JET_PACK_HEIGHT 72

#define BULLET_WIDTH 20
#define BULLET_HEIGHT 20

// jump and fly distance
#define BASIC_JUMP 320
#define SPRING_JUMP 960
#define TRAMPOLINE_JUMP 1920
#define PROPELLER_HAT_FLY 3840
#define JETPACK_FLY 7680

#define MAX_STAIR_INTERVAL 200 // 270
#define MIN_STAIR_INTERVAL 60  // 60

#define FLY 3
#define JUMP 2
#define RISE 1
#define FALL 0

#define LEFT 11
#define RIGHT 12
#define NO_LR 10

// unlock_score
#define UNLOCK_SPRING_SCORE 3000        // 3000
#define UNLOCK_TRAMPOLINE_SCORE 6000    // 6000
#define UNLOCK_PROPELLER_HAT_SCORE 9000 // 9000
#define UNLOCK_JET_PACK_SCORE 12000    // 12000

#define UNLOCK_MOVING_PLATFORM 6000       // 6000
#define UNLOCK_DISAPPEARING_PLATFORM 9000 // 9000

#define UNLOCK_MONSTER_SCORE 2000

// player setting
#define PLAYER_START_POSITION_X 200
#define PLAYER_START_POSITION_Y 500

#define PLAYER_MOVING_SPEED 10 // left right
#define PLAYER_FALLING_SPEED 25

#define PLAYER_RISING_SPEED 25
#define PLAYER_RISING_TIMES 24

#define PLAYER_JUMPING_SPEED 22
#define PLAYER_SPRING_JUMPING_SPEED 44
#define PLAYER_SPRING_JUMPING_TIMES 43
#define PLAYER_TRAMPOLINE_JUMPING_SPEED 61
#define PLAYER_TRAMPOLINE_JUMPING_TIMES 60

#define PLAYER_FLYING_SPEED 45
#define PLAYER_PROPELLER_HAT_FLYING__SPEED 50
#define PLAYER_PROPELLER_HAT_FLYING_TIMES 70 + 10
#define PLAYER_JETPACK_FLYING__SPEED 70
#define PLAYER_JETPACK_FLYING_TIMES 102 + 10
#define PLAYER_FLY_SLOW_DOWN_RATE 0.8

#define PLAYER_ACCELRATION 1

#define LR_DISTANCE 120

// stair setting
#define STAIR_FALLING_SPEED 4
#define STAIR_MOVING_SPEED 3
#define MAX_STAIR_MOVING_PIXEL 200
#define MIN_STAIR_MOVING_PIXEL 40

#define STAIR_FALLING_ACCELERATION 3
#define STAIR_RATE 35

// item setting
#define ITEM_RATE 30
#define MONSTER_RATE 5

// default setting
#define DEFAULT_HEALTH 3
#define DEFAULT_SCORE 0

// scene setting
#define HEALTH_POSITION_X 400
#define HEALTH_POSITION_Y 20

#define SCORE_POSITION_X 20
#define SCORE_POSITION_Y 20

// change item image
#define DONT_CARE 100
#define SPRING1 101
#define SPRING2 102
#define TRAMPOLINE1 111
#define TRAMPOLINE2 112
#define TRAMPOLINE3 113

// change doodle image code
#define DOODLE_RISE 0
#define DOODLE_FALL 1
#define DOODLEL 11
#define DOODLER 12
#define DOODLEUL 13
#define DOODLEUR 14
#define DOODLEH1 21
#define DOODLEH2 22
#define DOODLEH3 23
#define DOODLEH4 24
#define DOODLEJ1 31
#define DOODLEJ2 32
#define DOODLEJ3 33
#define DOODLEJ4 34
#define DOODLEJ5 35
#define DOODLEJ6 36
#define DOODLEJ7 37
#define DOODLEJ8 38
#define DOODLEJ9 39
#define DOODLEJ10 40

enum ITEM_ORDER
{
    // buttom
    STAIR_ITEM_ORDER,
    BROKEN_STAIR_ITEM_ORDER,
    PLAYER_ITEM_ORDER,
    TEXT_ITEM_ORDER,
    START_SCENE
    // top
};

enum StairType
{
    stair_basic,
    stair_moving,
    stair_disappear,
    stair_broken,
    NUM_OF_STAIR_TYPE, // This value is intented to keep the number of elements in this enum.
    stair_delete
};

enum ItemType
{
    spring,
    trampoline,
    propeller_hat,
    jet_pack,
    NUM_OF_ITEM_TYPE, // This value is intented to keep the number of elements in this enum.
    item_delete
};

enum MonsterType
{
    type1,
    type2,
    type3,
    type4,
    NUM_OF_MONSTER_TYPE, // This value is intented to keep the number of elements in this enum.
    monster_delete
};

// special image size
#define STAIR_BROKEN2_WIDTH 125
#define STAIR_BROKEN2_HEIGHT 40

#define STAIR_BROKEN3_WIDTH 125
#define STAIR_BROKEN3_HEIGHT 57

#define STAIR_BROKEN4_WIDTH 125
#define STAIR_BROKEN4_HEIGHT 64

#define SPRING2_WIDTH 40
#define SPRING2_HEIGHT 60

//monster image size

#define MONSTER_TYPE1_WIDTH 168
#define MONSTER_TYPE1_HEIGHT 105
#define MONSTER_TYPE2_WIDTH 128
#define MONSTER_TYPE2_HEIGHT 178
#define MONSTER_TYPE3_WIDTH 160
#define MONSTER_TYPE3_HEIGHT 100
#define MONSTER_TYPE4_WIDTH 160
#define MONSTER_TYPE4_HEIGHT 90



#endif // PARAMETER_H
