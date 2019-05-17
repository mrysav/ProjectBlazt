#include <stdio.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

#include "menu.h"
#include "input.h"
#include "state.h"
#include "background.h"

const GameState MENU_STATE = { &menu_loadResources, &menu_processInput, &menu_updateFrame, &menu_unloadResources };

MenuState menuState = { -1 };

ALLEGRO_FONT* font;

void menu_loadResources() {

    font = al_create_builtin_font();
    if (!font) {
        printf("could not load font. it's probably fine");
    }
}

void menu_unloadResources() {
    al_destroy_font(font);
}

State menu_processInput(unsigned char* keys) {

    if(keys[ALLEGRO_KEY_SPACE] == KEY_PRESSED) {
        return GAME;
    }

    if(keys[ALLEGRO_KEY_ESCAPE] == KEY_PRESSED) {
        return EXIT;
    }

    return MENU;
}

void menu_updateFrame() {

    drawBackground(BLACK);

    al_draw_text(font, al_map_rgb(255,255,255), 50, 100, 0, "Press space to play");

    return;
}
