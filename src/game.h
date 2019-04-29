#ifndef __GAME_H__
#define __GAME_H__

#include "state.h"

typedef struct GamePlayState {
    int x;
    int y;
    int vx;
    int vy;
} GamePlayState;

const GameState GAME_STATE;

State game_processInput(unsigned char* keys);

void game_updateFrame();

void game_loadResources();

void game_unloadResources();

#endif
