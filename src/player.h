# pragma once

#include <stdbool.h>
#include <stdint.h>

#include "util.h"

// TODO: Maybe define spritesheet here?

typedef struct Player {
    int_fast32_t firstFrame;
    int_fast32_t animFrame;
    bool isJumping;
    bool facingLeft;
    bool isMoving;
    Rect32 position;
    Rect32 hitbox;
} Player;

Player player_init();
void player_destroy(Player* player);

void player_tick(Player* player);

void player_draw(Player* player, Rect32* camera);
