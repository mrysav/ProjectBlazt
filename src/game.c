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
#define JUMP_VEL -12;

// Image resources
ALLEGRO_BITMAP *tilemap;

Player player;

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

void loadLevel(const Level* level) {
    currentLevel = level;
    map_total_width = currentLevel->width * TILE_WIDTH;
    map_total_height = currentLevel->height * TILE_HEIGHT;
}

void game_loadResources()
{
    tilemap = al_load_bitmap("image/tiles-alpha.png");

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
    player_destroy(&player);
}

State game_processInput(unsigned char *keys)
{
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

    if (xvel != 0)
    {
        int_fast32_t tileX = (xB + xvel) / TILE_WIDTH;
        int_fast32_t topTileY = player.hitbox.y / TILE_HEIGHT;
        int_fast32_t botTileY = (player.hitbox.y + player.hitbox.height - 1) / TILE_HEIGHT;
        bool collides = false;
        for (int t = topTileY; t <= botTileY; t++)
        {
            if (currentLevel->tiles[t][tileX] > 0)
            {
                collides = true;
                break;
            }
        }
        if (!collides)
        {
            player.hitbox.x += xvel;
            player.position.x += xvel;
        }
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

    if (yvel != 0)
    {
        int tileY = (yB + yvel) / TILE_HEIGHT;
        int leftTileX = player.hitbox.x / TILE_WIDTH;
        int rightTileX = (player.hitbox.x + player.hitbox.width - 1) / TILE_WIDTH;
        bool collides = false;
        float dist = yvel;
        for (int t = leftTileX; t <= rightTileX; t++)
        {
            if (currentLevel->tiles[tileY][t] > 0)
            {
                collides = true;
                yvel = 0;

                if (player.hitbox.y >= (tileY * TILE_HEIGHT + TILE_HEIGHT))
                {
                    dist = (tileY * TILE_HEIGHT + TILE_HEIGHT) - player.hitbox.y;
                }
                else
                {
                    // player is jumping and hit the tile above them
                    dist = (tileY * TILE_HEIGHT) - (player.hitbox.y + player.hitbox.height);
                    isJumping = false;
                }

                break;
            }
        }

        if (dist > 0 && !isJumping) {
            // player is falling, so say they are jumping
            // so they can't jump while falling
            isJumping = true;
        }
        player.hitbox.y += dist;
        player.position.y += dist;
    }

    player.isMoving = (xvel != 0) || (yvel != 0);
    player.isJumping = isJumping;

    player_tick(&player);

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
}

const GameState GAME_STATE = {
    &game_loadResources,
    &game_processInput,
    &game_updateFrame,
    &game_unloadResources
};
