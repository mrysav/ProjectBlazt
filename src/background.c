#include <stdio.h>

#include <allegro5/allegro5.h>

#include "background.h"

void background_draw_black() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
}

void background_draw_sky() {
    // cornflower blue
    al_clear_to_color(al_map_rgb(100, 149, 237));
}
