#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "input.h"
#include "game.h"
#include "background.h"
#include "util.h"
#include "display.h"
#include "player.h"
#include "npc.h"
#include "red_dude.h"

#include "level.h"
#include "level1.h"
#include "level2.h"


#define TS_WIDTH 4
#define TS_HEIGHT 28
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
// only draw up to one level of tiles offscreen
#define MIN_DRAW -16
#define MAX_DRAW 16

const Level* currentLevel;

#define GRAVITY 6
#define RUN_VEL 3
#define JUMP_VEL -12
#define MAX_NPC_COUNT 50

// Image resources
ALLEGRO_BITMAP *tilemap;
// Shared spritesheet
ALLEGRO_BITMAP *enem_sprites;

Player player;
NPC enemies[MAX_NPC_COUNT] = {0};

// Set when loadResources() is called
uint_fast32_t map_total_width;
uint_fast32_t map_total_height;
uint_fast32_t max_draw_x;
uint_fast32_t max_draw_y;
uint_fast32_t max_move_x;
uint_fast32_t max_move_y;
int_fast32_t player_x_offset;
int_fast32_t player_y_offset;
Rect32 camera = {0};

int_fast16_t numEnem = 0;

void loadLevel(const Level* level) {
    currentLevel = level;
    map_total_width = currentLevel->width * TILE_WIDTH;
    map_total_height = currentLevel->height * TILE_HEIGHT;
}

void game_loadResources()
{
    tilemap = al_load_bitmap("image/tiles-alpha.png");
    enem_sprites = al_load_bitmap("image/man-and-enemies.png");

    loadLevel(&LEVEL2);

    player = player_init();

    max_draw_x = map_total_width + MAX_DRAW;
    max_draw_y = map_total_height + MAX_DRAW;

    max_move_x = map_total_width - player.hitbox.width;
    max_move_y = map_total_height - player.hitbox.height;

    player_x_offset = -((SCREEN_WIDTH / 2) - (player.position.width / 2));
    player_y_offset = -((SCREEN_HEIGHT / 2) - (player.position.height / 2));
    
    camera.x = player_x_offset;
    camera.y = player_y_offset;
    camera.width = SCREEN_WIDTH;
    camera.height = SCREEN_HEIGHT;
}

void game_unloadResources()
{
    al_destroy_bitmap(tilemap);
    al_destroy_bitmap(enem_sprites);
    player_destroy(&player);
}

void spawnEnemy() {
    if (numEnem >= 1) {
        return;
    }
    enemies[numEnem] = red_dude;
    Vec32 pos = {0,0};
    enemies[numEnem].init(&enemies[numEnem], pos);
    numEnem++;
}

gravdata calc_collide(int_fast32_t xvel, int_fast32_t xedge, int_fast32_t x, int_fast32_t width, 
                      int_fast32_t yvel, int_fast32_t yedge, int_fast32_t y, int_fast32_t height) {
    
    gravdata res = {0};
    
    if (xvel != 0) 
    {
        int_fast32_t tileX = (xedge + xvel) / TILE_WIDTH;
        int_fast32_t topTileY = y / TILE_HEIGHT;
        int_fast32_t botTileY = (y + height - 1) / TILE_HEIGHT;
        bool collides = false;
        for (int_fast32_t t = topTileY; t <= botTileY; t++)
        {
            if (currentLevel->tiles[t][tileX] > 0)
            {
                collides = true;
                break;
            }
        }
        if (!collides)
        {
            res.xdist = xvel;
        }
    }

    if (yvel != 0)
    {
        int_fast32_t tileY = (yedge + yvel) / TILE_HEIGHT;
        int_fast32_t leftTileX = x / TILE_WIDTH;
        int_fast32_t rightTileX = (x + width - 1) / TILE_WIDTH;
        int_fast32_t dist = yvel;
        for (int_fast32_t t = leftTileX; t <= rightTileX; t++)
        {
            if (currentLevel->tiles[tileY][t] > 0)
            {
                int_fast32_t tile_realY = tileY * TILE_HEIGHT;

                // entity is falling and hits the floor
                if (y >= (tile_realY + TILE_HEIGHT))
                {
                    dist = (tile_realY + TILE_HEIGHT) - y;
                    res.hit_floor = true;
                }
                // entity is jumping and hit the tile above them
                else
                {
                    dist = tile_realY - (y + height);
                    res.hit_top = true;
                }

                break;
            }
        }

        res.ydist = dist;
    }

    return res;
}

State game_processInput(unsigned char *keys)
{
    spawnEnemy();

    static bool isJumping;
    static int_fast32_t yvel;

    if (keys[ALLEGRO_KEY_ESCAPE] == KEY_PRESSED)
    {
        return MENU;
    }

    int_fast32_t xB = 0;
    int_fast32_t xvel = 0;
    if (keys[ALLEGRO_KEY_RIGHT] == KEY_HELD)
    {
        xB = player.hitbox.x + player.hitbox.width - 1;
        xvel = RUN_VEL;
    }
    else if (keys[ALLEGRO_KEY_LEFT] == KEY_HELD)
    {
        xB = player.hitbox.x;
        xvel = -RUN_VEL;
    }

    if (xvel < 0) {
        player.facingLeft = true;
    } else if (xvel > 0) {
        player.facingLeft = false;
    }

    int_fast32_t yB = 0;
    if (keys[ALLEGRO_KEY_UP] == KEY_PRESSED && !isJumping)
    {
        yvel = JUMP_VEL;
        isJumping = true;
    }
    else
    {
        yvel = min(yvel + 1, GRAVITY);
    }

    if (yvel <= 0)
    {
        yB = player.hitbox.y;
    }
    else
    {
        yB = player.hitbox.y + player.hitbox.height - 1;
    }

    gravdata dat = calc_collide(xvel, xB, player.hitbox.x, player.hitbox.width,
                                yvel, yB, player.hitbox.y, player.hitbox.height);

    if (isJumping) {
        if (dat.hit_floor || dat.ydist == 0) {
            isJumping = false;
        }
    } else {
        if (dat.ydist != 0) {
            isJumping = true;
        }
    }

    player.hitbox.x += dat.xdist;
    player.position.x += dat.xdist;

    player.hitbox.y += dat.ydist;
    player.position.y += dat.ydist;

    player.isMoving = (xvel != 0) || (dat.ydist != 0);
    player.isJumping = isJumping;

    player_tick(&player);

    for(uint_fast16_t i = 0; i < MAX_NPC_COUNT; i++) {
        if (enemies[i].isInit) {
            enemies[i].tick(&enemies[i], calc_collide);
        }
    }

    camera.x = player.position.x + player_x_offset;
    camera.y = player.position.y + player_y_offset;

    return GAME;
}

void drawLevel()
{
    for (int y = 0; y < currentLevel->height; y++)
    {
        float posY = y * TILE_HEIGHT - camera.y;
        if (posY >= MIN_DRAW && posY <= max_draw_y) {
            for (int x = 0; x < currentLevel->width; x++)
            {
                float posX = x * TILE_WIDTH - camera.x;
                if (posX >= MIN_DRAW && posY <= max_draw_x) {
                    int spriteId = currentLevel->tiles[y][x] - 1;
                    if (spriteId >= 0)
                    {
                        int tx = spriteId % TS_WIDTH * TILE_WIDTH;
                        int ty = spriteId / TS_WIDTH * TILE_HEIGHT;
                        al_draw_bitmap_region(tilemap, tx, ty, TILE_WIDTH, TILE_HEIGHT, posX, posY, 0);
                    }
                }
            }
        }
    }
}

void game_updateFrame()
{
    background_draw_sky();

    drawLevel();

    player_draw(&player, &camera);

    for(uint_fast16_t i = 0; i < MAX_NPC_COUNT; i++) {
        if (enemies[i].isInit) {
            enemies[i].draw(&enemies[i], &camera, enem_sprites);
        }
    }
}

const GameState GAME_STATE = {
    &game_loadResources,
    &game_processInput,
    &game_updateFrame,
    &game_unloadResources
};
