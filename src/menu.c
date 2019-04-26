#include <stdio.h>
#include "menu.h"

const GameState MenuState = { &menu_processInput, &menu_updateFrame };

void menu_processInput(int* keys) {
    printf("menu_processInput");
    return;
}

void menu_updateFrame() {
    return;
}
