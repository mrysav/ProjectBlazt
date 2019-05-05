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
    { 0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 2,3,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, },
};

#define TILE_WIDTH 64
#define TILE_HEIGHT 64
static const int MAX_TILES_X = SCREEN_WIDTH / TILE_WIDTH;
static const int MAX_TILES_Y = SCREEN_HEIGHT / TILE_HEIGHT;

GamePlayState state = { };
const float gravity = 9.0;

Rectangle camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

// Image resources
ALLEGRO_BITMAP* tilemap;
ALLEGRO_BITMAP* player;


void game_loadResources() {
    tilemap = al_load_bitmap("image/tiles-alpha.png");
}

void game_unloadResources() {
    al_destroy_bitmap(tilemap);
}

State game_processInput(unsigned char* keys) {

    if(keys[ALLEGRO_KEY_UP] == KEY_HELD) {
        state.y--;
        camera.y--;
    }

    if(keys[ALLEGRO_KEY_DOWN] == KEY_HELD) {
        state.y++;
        camera.y++;
    }

    if(keys[ALLEGRO_KEY_RIGHT] == KEY_HELD) {
        state.x++;
        camera.x++;
    }

    if(keys[ALLEGRO_KEY_LEFT] == KEY_HELD) {
        state.x--;
        camera.x--;
    }

    if(keys[ALLEGRO_KEY_ESCAPE] == KEY_PRESSED) {
        return MENU;
    }

    return GAME;
}

void drawLevel() {
    for(int y = 0; y < LEVEL_1_HEIGHT; y++) {
        float posY = y * TILE_HEIGHT - camera.y;
        for(int x = 0; x < LEVEL_1_WIDTH; x++) {
            float posX = x * TILE_WIDTH - camera.x;
            int spriteId = LEVEL_1[y][x];
            al_draw_bitmap_region(tilemap, 0, spriteId * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, posX, posY, 0);
        }
    }
}

void game_updateFrame() {

    drawBackground(SKY);

    drawLevel();

    al_draw_filled_rectangle(state.x, state.y, state.x + 40, state.y + 40, al_map_rgb(255, 0, 0));
}
