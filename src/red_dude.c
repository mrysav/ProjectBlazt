#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

#include "red_dude.h"

#include "util.h"
#include "npc.h"

#define RED_INIT_HEALTH 50

// Sprite IDs + animations
#define STANDING_LEFT 5
#define WALKING_LEFT 16
#define WALKING_LEFT_LEN 6

// Spritesheet information
#define SHEET_HEIGHT 5
#define SHEET_WIDTH 11

#define RED_WIDTH 16
#define RED_HEIGHT 16
#define RED_HIT_WIDTH 16
#define RED_HIT_HEIGHT 10
#define RED_HIT_OFF_X 0
#define RED_HIT_OFF_Y 6

void init_red_dude(NPC* this, Vec32 pos) {
    this->health = RED_INIT_HEALTH;

    Rect32 rect = { pos.x, pos.y, RED_WIDTH, RED_HEIGHT };
    this->position = rect;

    Rect32 hitbox = { pos.x + RED_HIT_OFF_X, pos.y + RED_HIT_OFF_Y, RED_HIT_WIDTH, RED_HIT_HEIGHT };
    this->hitbox = hitbox;

    this->firstFrame = STANDING_LEFT;
    this->animFrame = STANDING_LEFT;
    this->animSeq = 0;
    this->isJumping = false;
    this->facingLeft = false;
    this->isMoving = false;

    this->isInit = true;
}

void tick_red_dude(NPC* this, jumpfunc jump, int_fast32_t delay) {

    // de brainzz
    int_fast32_t xvel;
    int_fast32_t xedge;
    if (this->facingLeft) {
        xvel = -1;
        xedge = this->hitbox.x;
    } else {
        xvel = 1;
        xedge = this->hitbox.x + this->hitbox.width;
    }

    int_fast32_t yvel = 3;
    int_fast32_t yedge = this->hitbox.y + this->hitbox.height;

    this->isMoving = true;

    gravdata dat = jump(xvel, xedge, this->hitbox.x, this->hitbox.width, yvel, yedge, this->hitbox.y, this->hitbox.height);

    if (xvel != dat.xdist) {
        this->facingLeft = !this->facingLeft;
    } else {
        this->facingLeft = dat.xdist < 0;
    }

    this->hitbox.x += dat.xdist;
    this->position.x += dat.xdist;

    this->hitbox.y += dat.ydist;
    this->position.y += dat.ydist;

    // Only update the animation frame once every 4 frames
    if ((delay % 4) > 0) {
        return;
    }

    if (this->isMoving) {
        this->firstFrame = WALKING_LEFT;
        this->animSeq = ++this->animSeq % WALKING_LEFT_LEN;
        this->animFrame = this->firstFrame + this->animSeq;
    } else {
        this->firstFrame = STANDING_LEFT;
        this->animFrame = 0;
    }
}

void collide_red_dude(NPC* this, uint_fast8_t x, uint_fast8_t y) {

}

void draw_red_dude(NPC* this, Rect32* camera, ALLEGRO_BITMAP* spritesheet) {
    int_fast32_t x1 = this->position.x - camera->x;
    int_fast32_t y1 = this->position.y - camera->y;

    int_fast32_t spriteId = this->animFrame;
    int_fast32_t tx = spriteId % SHEET_WIDTH * RED_WIDTH;
    int_fast32_t ty = spriteId / SHEET_WIDTH * RED_HEIGHT;

    int flags = this->facingLeft ? 0 : ALLEGRO_FLIP_HORIZONTAL;

    al_draw_bitmap_region(spritesheet, tx, ty, RED_WIDTH, RED_HEIGHT, x1, y1, flags);
}

NPC red_dude = {
    &init_red_dude,
    &tick_red_dude,
    &collide_red_dude,
    &draw_red_dude
};
