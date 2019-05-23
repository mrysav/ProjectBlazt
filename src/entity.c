#include "entity.h"

#define STANDING_RIGHT 1
#define WALKING_RIGHT 0
#define WALKING_RIGHT_LEN 3
#define JUMPING_RIGHT 3

#define STANDING_LEFT 6
#define WALKING_LEFT 5
#define WALKING_LEFT_LEN 3
#define JUMPING_LEFT 4

_Player Player = { 0, 0, false, false, false, { 0,0,0,0 }, { 0,0,0,0 }};

void player_tick(_Player* player) {
    static int delay = 0;
    // 8 only animation once every 8th frame
    delay = (++delay) % 6;
    if (delay > 1) {
        return;
    } 

    if (player->isMoving) {
        if (player->isJumping) {
            player->firstFrame = player->facingLeft ? JUMPING_LEFT : JUMPING_RIGHT;
            player->animFrame = 0;
        } else {
            player->firstFrame = player->facingLeft ? WALKING_LEFT : WALKING_RIGHT;
            player->animFrame = (++player->animFrame) % (player->facingLeft ? WALKING_LEFT_LEN : WALKING_RIGHT_LEN);
        }
    } else {
        player->firstFrame = player->facingLeft ? STANDING_LEFT : STANDING_RIGHT;
        player->animFrame = 0;
    }
}

void player_init(_Player* player) {
    // init things here i guess
}
