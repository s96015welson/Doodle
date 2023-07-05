#ifndef PARAMETER_H
#define PARAMETER_H

#define FRAME_DELAY 50

//image size
#define SCREEN_WIDTH 540
#define SCREEN_HEIGHT 960

#define DOODLE_WIDTH 80
#define DOODLE_HEIGHT 80

#define STAIR_WIDTH 120
#define STAIR_HEIGHT 30

#define SPRING_WIDTH 40
#define SPRING_HEIGHT 25

#define trampoline_width 72
#define trampoline_height 36

#define propeller_hat_width 64
#define propeller_hat_height 40

#define jet_pack_width 50
#define jet_pack_height 72

#define bullet_width 20
#define bullet_height 20


//jump and fly distance
#define BASIC_JUMP 320
#define SPRING_JUMP 960
#define TRAMPOLINE_JUMP 1920
#define PROPELLER_HAT_FLY 3840
#define JETPACK_FLY 7680

#define MAX_STAIR_INTERVAL 270
#define MIN_STAIR_INTERVAL 60

#define FLY 3
#define JUMP 2
#define RISE 1
#define FALL 0

#define LEFT 11
#define RIGHT 12
#define NO_LR 10


//unlock_score
# define unlock_spring_score 3000
# define unlock_trampoline_score 6000
# define unlock_propeller_hat_score 9000
# define unlock_jet_pack_score 12000

# define UNLOCK_MOVING_PLATFORM 6000
# define UNLOCK_DISAPPEARING_PLATFORM 9000

//default player setting
#define PLAYER_START_POSITION_X 100
#define PLAYER_START_POSITION_Y 600

#define PLAYER_MOVING_SPEED 10 
#define PLAYER_RISING_SPEED 40 
#define PLAYER_FALLING_SPEED 5
#define PLAYER_JUMPING_SPEED 50
#define PLAYER_FLYING_SPEED 100
#define PLAYER_ACCELRATION 1

#define LR_DISTANCE 80


//stair setting
#define STAIR_FALLING_SPEED 2

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
    PLAYER_ITEM_ORDER,
    TEXT_ITEM_ORDER 
    // top
};



#endif // PARAMETER_H