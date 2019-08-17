#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "util.h"

// TODO: grosssssss
typedef struct gravdata {
    bool hit_top;
    bool hit_floor;
    int_fast32_t xdist;
    int_fast32_t ydist;
} gravdata;

// TODO: ewwwwwwwww
typedef gravdata (*jumpfunc) (int_fast32_t xvel, int_fast32_t xedge, int_fast32_t x, int_fast32_t width, int_fast32_t yvel, int_fast32_t yedge, int_fast32_t y, int_fast32_t height);

typedef struct NPC {
    void (*init) (struct NPC* npc, Vec32 pos);
    void (*tick) (struct NPC* npc, jumpfunc jump, int_fast32_t delay);
    void (*collide) (struct NPC* npc, uint_fast8_t x, uint_fast8_t y);
    void (*draw) (struct NPC* npc, Rect32* camera, ALLEGRO_BITMAP* spritesheet);

    bool isInit;

    int_fast8_t firstFrame;
    int_fast8_t animSeq;
    int_fast8_t animFrame;
    bool isJumping;
    bool facingLeft;
    bool isMoving;

    Rect32 position;
    Rect32 hitbox;

    uint_fast32_t health;
} NPC;


