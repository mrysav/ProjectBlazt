#include <stdio.h>
#include <stdbool.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "input.h"
#include "game.h"
#include "background.h"
#include "util.h"
#include "display.h"

#include "level.h"
#include "level1.h"
#include "level2.h"

const GameState GAME_STATE = {
    &game_loadResources,
    &game_processInput,
    &game_updateFrame,
    &game_unloadResources};

#define TS_WIDTH 4
#define TS_HEIGHT 28
#define TILE_WIDTH 16
#define TILE_HEIGHT 16
// only draw up to one level of tiles offscreen
#define MIN_DRAW -16
#define MAX_DRAW 16
#define PLAYER_HEIGHT 25
#define PLAYER_WIDTH 10

const static float PLAYER_X_OFFSET = (SCREEN_WIDTH / 2) - (PLAYER_WIDTH / 2);
const static float PLAYER_Y_OFFSET = (SCREEN_HEIGHT / 2) - (PLAYER_HEIGHT / 2);

const Level* currentLevel;

const static float gravity = 4.0;
const static float runSpeed = 2;
const static float jumpVel = -12.0;

bool isJumping = false;
float yvel = 0;

RectangleF playerBox = {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT};
RectangleF camera = {0 - PLAYER_X_OFFSET, 0 - PLAYER_Y_OFFSET, SCREEN_WIDTH, SCREEN_HEIGHT};

int max_draw_x;
int max_draw_y;
int max_move_x;
int max_move_y;

// Image resources
ALLEGRO_BITMAP *tilemap;
ALLEGRO_BITMAP *player;

void game_loadResources()
{
    tilemap = al_load_bitmap("image/tiles-alpha.png");

    currentLevel = &LEVEL2;

    max_draw_x = currentLevel->width * TILE_WIDTH + MAX_DRAW;
    max_draw_y = currentLevel->height * TILE_HEIGHT + MAX_DRAW;

    max_move_x = currentLevel->width * TILE_WIDTH - PLAYER_WIDTH;
    max_move_y = currentLevel->height * TILE_WIDTH - PLAYER_HEIGHT;

}

void game_unloadResources()
{
    al_destroy_bitmap(tilemap);

    isJumping = false;
    yvel = 0;
    playerBox.x = 0;
    playerBox.y = 0;
}

State game_processInput(unsigned char *keys)
{
    if (keys[ALLEGRO_KEY_ESCAPE] == KEY_PRESSED)
    {
        return MENU;
    }

    float xB = 0;
    float xvel = 0;
    if (keys[ALLEGRO_KEY_RIGHT] == KEY_HELD)
    {
        xB = playerBox.x + playerBox.width - 1;
        xvel = runSpeed;
    }
    else if (keys[ALLEGRO_KEY_LEFT] == KEY_HELD)
    {
        xB = playerBox.x;
        xvel = -runSpeed;
    }

    if (xvel != 0)
    {
        int tileX = (xB + xvel) / TILE_WIDTH;
        int topTileY = playerBox.y / TILE_HEIGHT;
        int botTileY = (playerBox.y + playerBox.height - 1) / TILE_HEIGHT;
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
            playerBox.x += xvel;
        }
    }

    float yB = 0;
    if (keys[ALLEGRO_KEY_UP] == KEY_PRESSED && !isJumping)
    {
        yvel = -12;
        isJumping = true;
    }
    else
    {
        yvel += 1;
        if (yvel > gravity)
        {
            yvel = gravity;
        }
    }

    if (yvel <= 0)
    {
        yB = playerBox.y;
    }
    else
    {
        yB = playerBox.y + playerBox.height - 1;
    }

    if (yvel != 0)
    {
        int tileY = (yB + yvel) / TILE_HEIGHT;
        int leftTileX = playerBox.x / TILE_WIDTH;
        int rightTileX = (playerBox.x + playerBox.width - 1) / TILE_WIDTH;
        bool collides = false;
        float dist = yvel;
        for (int t = leftTileX; t <= rightTileX; t++)
        {
            if (currentLevel->tiles[tileY][t] > 0)
            {
                collides = true;
                yvel = 0;

                if (playerBox.y >= (tileY * TILE_HEIGHT + TILE_HEIGHT))
                {
                    dist = (tileY * TILE_HEIGHT + TILE_HEIGHT) - playerBox.y;
                }
                else
                {
                    // player is jumping and hit the tile above them
                    dist = (tileY * TILE_HEIGHT) - (playerBox.y + playerBox.height);
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
        playerBox.y += dist;
    }

    if (playerBox.x < 0)
    {
        playerBox.x = 0;
    } 
    else if (playerBox.x > max_move_x)
    {
        playerBox.x = max_move_x;
    }

    if (playerBox.y < 0)
    {
        playerBox.y = 0;
    }
    else if (playerBox.y > max_move_y)
    {
        playerBox.y = max_move_y;
    }

    camera.x = playerBox.x - PLAYER_X_OFFSET;
    camera.y = playerBox.y - PLAYER_Y_OFFSET
;

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

void drawPlayer()
{
    float x1 = playerBox.x - camera.x;
    float x2 = x1 + playerBox.width;
    float y1 = playerBox.y - camera.y;
    float y2 = y1 + playerBox.height;
    al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0, 0, 0));
}

void game_updateFrame()
{
    drawBackground(SKY);

    drawLevel();

    drawPlayer();
}
