#include <stdint.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "player.h"

// Sprite IDs + animations
#define JUMPING_RIGHT 3
#define STANDING_RIGHT 1
#define WALKING_RIGHT 0
#define WALKING_RIGHT_LEN 2
#define JUMPING_LEFT 4
#define STANDING_LEFT 6
#define WALKING_LEFT 5
#define WALKING_LEFT_LEN 2

// Spritesheet information
#define SPRITE_HEIGHT 16
#define SPRITE_WIDTH 16
#define SHEET_HEIGHT 2
#define SHEET_WIDTH 4

// Hitbox info
#define HITBOX_WIDTH 10
#define HITBOX_OFF_X 3
#define HITBOX_HEIGHT 16
#define HITBOX_OFF_Y 0

ALLEGRO_BITMAP *player_sheet;

Player player_init() {
    Player player = {0};

    player_sheet = al_load_bitmap("image/ddave-char-trans.png");

    player.position.x = 0;
    player.position.y = 0;
    player.position.width = SPRITE_WIDTH;
    player.position.height = SPRITE_HEIGHT;

    player.hitbox.x = player.position.x + HITBOX_OFF_X;
    player.hitbox.y = player.position.y + HITBOX_OFF_Y;
    player.hitbox.width = HITBOX_WIDTH;
    player.hitbox.height = HITBOX_HEIGHT;

    return player;
}

void player_destroy(Player* player) {
    al_destroy_bitmap(player_sheet);
    player->isJumping = false;
    player->firstFrame = 0;
    player->animFrame = 0;
    player->isMoving = false;
}

void player_tick(Player* player) {
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

void player_draw(Player* player, Rect32* camera) {
    int_fast32_t x1 = player->position.x - camera->x;
    int_fast32_t y1 = player->position.y - camera->y;

    int_fast32_t spriteId = player->firstFrame + player->animFrame;
    int_fast32_t tx = spriteId % SHEET_WIDTH * SPRITE_WIDTH;
    int_fast32_t ty = spriteId / SHEET_WIDTH * SPRITE_HEIGHT;

    al_draw_bitmap_region(player_sheet, tx, ty, SPRITE_WIDTH, SPRITE_HEIGHT, x1, y1, 0);
}
