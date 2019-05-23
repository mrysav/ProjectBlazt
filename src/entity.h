#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <stdbool.h>

#include "util.h"

// TODO: Maybe define spritesheet here?

typedef struct _Player {
    int firstFrame;
    int animFrame;
    bool isJumping;
    bool facingLeft;
    bool isMoving;
    // unused
    RectangleF position;
    RectangleF hitbox;
} _Player;

extern _Player Player;

void player_init(_Player* player);
void player_tick(_Player* player);

#endif
