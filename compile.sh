#!/bin/sh

gcc game.c -lm -o bin/game $(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)
