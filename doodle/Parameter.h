#ifndef PARAMETER_H
#define PARAMETER_H

#define FRAME_DELAY 50

//basic image size
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





//jump and fly distance
#define BASIC_JUMP 320
#define SPRING_JUMP 960
#define TRAMPOLINE_JUMP 1920
#define PROPELLER_HAT_FLY 3840
#define JETPACK_FLY 7680

#define MAX_STAIR_INTERVAL 150  //200
#define MIN_STAIR_INTERVAL 60   //60
  
#define FLY 3
#define JUMP 2
#define RISE 1
#define FALL 0

#define LEFT 11
#define RIGHT 12
#define NO_LR 10


//unlock_score
# define UNLOCK_SPRING_SCORE 3000
# define UNLOCK_TRAMPOLINE_SCORE 6000
# define UNLOCK_PROPELLER_HAT_SCORE 9000
# define UNLOCK_JET_PACK_SCORE 12000

# define UNLOCK_MOVING_PLATFORM 600 //6000
# define UNLOCK_DISAPPEARING_PLATFORM 900 //9000

//player setting
#define PLAYER_START_POSITION_X 100
#define PLAYER_START_POSITION_Y 600

#define PLAYER_MOVING_SPEED 20 
#define PLAYER_RISING_SPEED 40 
#define PLAYER_FALLING_SPEED 10
#define PLAYER_JUMPING_SPEED 50
#define PLAYER_FLYING_SPEED 100
#define PLAYER_ACCELRATION 1

#define LR_DISTANCE 120


//stair setting
#define STAIR_FALLING_SPEED 5
#define STAIR_MOVING_SPEED 4
#define MAX_STAIR_MOVING_PIXEL 200
#define MIN_STAIR_MOVING_PIXEL 40

#define STAIR_FALLING_ACCELERATION 4
#define STAIR_RATE 35

// item setting
#define ITEM_RATE 10

//default setting
# define DEFAULT_HEALTH 3
# define DEFAULT_SCORE 0 

//scene setting
#define HEALTH_POSITION_X 400
#define HEALTH_POSITION_Y 20

#define SCORE_POSITION_X 20
#define SCORE_POSITION_Y 20




enum ITEM_ORDER {
    // buttom
    STAIR_ITEM_ORDER,
    BROKEN_STAIR_ITEM_ORDER,
    PLAYER_ITEM_ORDER,
    TEXT_ITEM_ORDER 
    // top
};

enum StairType
{
    stair_basic,
    stair_broken,
    stair_moving,
    stair_disappear,
    NUM_OF_STAIR_TYPE, // This value is intented to keep the number of elements in this enum.
    stair_delete
};

enum ItemType
{
    spring,
    trampoline,
    propeller_hat,
    jet_pack,
    NUM_OF_Item_TYPE, // This value is intented to keep the number of elements in this enum.
    item_delete,
};




//special image size
#define STAIR_BROKEN2_WIDTH 125
#define STAIR_BROKEN2_HEIGHT 40

#define STAIR_BROKEN3_WIDTH 125
#define STAIR_BROKEN3_HEIGHT 57

#define STAIR_BROKEN4_WIDTH 125
#define STAIR_BROKEN4_HEIGHT 64


#endif // PARAMETER_H
