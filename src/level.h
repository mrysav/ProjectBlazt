#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stdint.h>

// TODO: Can this be more dynamic?
#define LEVEL_MEMSIZE_X 100
#define LEVEL_MEMSIZE_Y 100

typedef struct Level {
    const int_fast32_t width;
    const int_fast32_t height;
    const int_fast32_t tiles[LEVEL_MEMSIZE_Y][LEVEL_MEMSIZE_X];
} Level;

#endif
