#include <stdio.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "input.h"
#include "game.h"
#include "background.h"

const GameState GAME_STATE = {
    &game_loadResources,
    &game_processInput,
    &game_updateFrame,
    &game_unloadResources
};

GamePlayState state = { };
const float gravity = 9.0;

void game_loadResources() {

}

void game_unloadResources() {

}

State game_processInput(unsigned char* keys) {

    if(keys[ALLEGRO_KEY_UP] == KEY_HELD) {
        state.y--;
    }

    if(keys[ALLEGRO_KEY_DOWN] == KEY_HELD) {
        state.y++;
    }

    if(keys[ALLEGRO_KEY_RIGHT] == KEY_HELD) {
        state.x++;
    }

    if(keys[ALLEGRO_KEY_LEFT] == KEY_HELD) {
        state.x--;
    }

    if(keys[ALLEGRO_KEY_ESCAPE] == KEY_PRESSED) {
        return MENU;
    }

    return GAME;
}

void game_updateFrame() {

    drawBackground(SKY);

    al_draw_filled_rectangle(state.x, state.y, state.x + 40, state.y + 40, al_map_rgb(255, 0, 0));
}
