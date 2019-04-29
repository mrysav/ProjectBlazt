#ifndef __MENU_H__
#define __MENU_H__

#include "state.h"

typedef struct MenuState {
    int selected;
} MenuState;

const GameState MENU_STATE;

State menu_processInput(unsigned char* keys);

void menu_updateFrame();

void menu_loadResources();

void menu_unloadResources();

#endif
