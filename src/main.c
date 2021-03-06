#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "menu.h"
#include "game.h"
#include "state.h"
#include "display.h"
#include "util.h"

// Defines

#define GAME_KEY_SEEN 1
#define GAME_KEY_RELEASED 2

// Constants

const GameState* GAME_STATES[] = { &MENU_STATE, &GAME_STATE };

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    // al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    al_set_new_display_flags(ALLEGRO_WINDOWED);

    // calculate scaling factor
    int sx = WINDOW_WIDTH / SCREEN_WIDTH;
    int sy = WINDOW_HEIGHT / SCREEN_HEIGHT;
    int scale = min(sx, sy);

    int scaleW = SCREEN_WIDTH * scale;
    int scaleH = SCREEN_HEIGHT * scale;
    int scaleX = (WINDOW_WIDTH - scaleW) / 2;
    int scaleY = (WINDOW_HEIGHT - scaleH) / 2;

    ALLEGRO_DISPLAY* disp = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    ALLEGRO_BITMAP* buffer = al_create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
    must_init(disp, "display");

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_init_image_addon(), "images");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool redraw = true;
    bool done = false;
    float x = 0;
    float y = 0;
    ALLEGRO_EVENT event;

    unsigned char key[ALLEGRO_KEY_MAX] = {0};

    al_start_timer(timer);

    State state = MENU;
    GAME_STATES[(int)state]->loadResources();

    while(1)
    {
        const GameState* gState = GAME_STATES[(int)state];
        State nextState = state;

        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:

                nextState = gState->processInput(key);

                if (nextState == EXIT) {
                    done = true;
                }

                for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= GAME_KEY_SEEN;

                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = GAME_KEY_SEEN | GAME_KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] &= GAME_KEY_RELEASED;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done) {
            gState->unloadResources();
            break;
        }

        if(redraw && al_is_event_queue_empty(queue))
        {
            // render a frame
            al_set_target_bitmap(buffer);
            // al_clear_to_color(al_map_rgb(0, 0, 0));

            gState->updateDisplay();

            al_set_target_backbuffer(disp);
            // sal_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_bitmap(buffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, scaleX, scaleY, scaleW, scaleH, 0);

            al_flip_display();

            redraw = false;
        }

        if (state != nextState) {
            const GameState* nState = GAME_STATES[(int)nextState];

            gState->unloadResources();
            nState->loadResources();

            state = nextState;
        }

        fflush(stdout);
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
