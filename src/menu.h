#ifndef __MENU_H__
#define __MENU_H__

#include "state.h"

void menu_processInput(int* keys);

void menu_updateFrame();

const GameState MenuState;

#endif
