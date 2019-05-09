#include <stdio.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "input.h"
#include "game.h"
#include "background.h"
#include "util.h"
#include "display.h"

const GameState GAME_STATE = {
    &game_loadResources,
    &game_processInput,
    &game_updateFrame,
    &game_unloadResources
};

#define LEVEL_1_WIDTH 20
#define LEVEL_1_HEIGHT 20
const int LEVEL_1[LEVEL_1_HEIGHT][LEVEL_1_WIDTH] = {
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 2,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,56,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,65,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,56,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,45,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
};

#define TS_WIDTH 4
#define TS_HEIGHT 28
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

// Player is centered
#define PLAYER_HEIGHT 16
#define PLAYER_WIDTH 16
const static float PLAYER_X1 = (SCREEN_WIDTH / 2) - (PLAYER_WIDTH / 2);
const static float PLAYER_Y1 = (SCREEN_HEIGHT / 2) - (PLAYER_HEIGHT / 2);
const static float PLAYER_X2 = PLAYER_X1 + PLAYER_WIDTH;
const static float PLAYER_Y2 = PLAYER_Y1 + PLAYER_HEIGHT;

const static float MIN_X = 0;
const static float MIN_Y = 0;
const static float MAX_X = LEVEL_1_WIDTH * TILE_WIDTH - PLAYER_WIDTH;
const static float MAX_Y = LEVEL_1_HEIGHT * TILE_WIDTH - PLAYER_HEIGHT;

const float gravity = 9.0;

RectangleF playerBox = { 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT };
RectangleF camera = { 0 - PLAYER_X1, 0 - PLAYER_Y1, SCREEN_WIDTH, SCREEN_HEIGHT };

// Image resources
ALLEGRO_BITMAP* tilemap;
ALLEGRO_BITMAP* player;


void game_loadResources() {
    tilemap = al_load_bitmap("image/tiles-alpha.png");
}

void game_unloadResources() {
    al_destroy_bitmap(tilemap);
}

void moveWithCollisionDetection(VectorF* targetSpeed) {

}

State game_processInput(unsigned char* keys) {

    if(keys[ALLEGRO_KEY_ESCAPE] == KEY_PRESSED) {
        return MENU;
    }

    float xB = 0;
    if(keys[ALLEGRO_KEY_RIGHT] == KEY_HELD) {
        playerBox.x++;
    } else if(keys[ALLEGRO_KEY_LEFT] == KEY_HELD) {
        playerBox.x--;
    }

    if(keys[ALLEGRO_KEY_UP] == KEY_HELD) {
        playerBox.y--;
    } else if(keys[ALLEGRO_KEY_DOWN] == KEY_HELD) {
        playerBox.y++;
    }

    if (playerBox.x < MIN_X) {
        playerBox.x = MIN_X;
    }
    if (playerBox.x > MAX_X) {
        playerBox.x = MAX_X;
    }
    if (playerBox.y < MIN_Y) {
        playerBox.y = MIN_Y;
    }
    if (playerBox.y > MAX_Y) {
        playerBox.y = MAX_Y;
    }

    camera.x = playerBox.x - PLAYER_X1;
    camera.y = playerBox.y - PLAYER_Y1;

    return GAME;
}

void drawLevel() {
    for(int y = 0; y < LEVEL_1_HEIGHT; y++) {
        float posY = y * TILE_HEIGHT - camera.y;
        for(int x = 0; x < LEVEL_1_WIDTH; x++) {
            float posX = x * TILE_WIDTH - camera.x;
            int spriteId = LEVEL_1[y][x] - 1;
            if (spriteId >= 0) {
                int tx = spriteId % TS_WIDTH * TILE_WIDTH;
                int ty = spriteId / TS_WIDTH * TILE_HEIGHT;
                al_draw_bitmap_region(tilemap, tx, ty, TILE_WIDTH, TILE_HEIGHT, posX, posY, 0);
            }
        }
    }
}

void drawPlayer() {
    float x1 = playerBox.x - camera.x;
    float x2 = x1 + playerBox.width;
    float y1 = playerBox.y - camera.y;
    float y2 = y1 + playerBox.height;
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0,0,0));
}

void game_updateFrame() {

    drawBackground(SKY);

    drawLevel();

    drawPlayer();
}
