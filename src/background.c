#include <stdio.h>

#include <allegro5/allegro5.h>

#include "background.h"

void drawBackground(Background bg) {
    switch(bg) {
        case BLACK:
            al_clear_to_color(al_map_rgb(0, 0, 0));
            break;
        case SKY:
            // cornflower blue
            al_clear_to_color(al_map_rgb(100, 149, 237));
            break;
        default:
            printf("Invalid background.\n");
    }
}
