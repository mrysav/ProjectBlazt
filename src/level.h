#ifndef __LEVEL_H__
#define __LEVEL_H__

// TODO: Can this be more dynamic?
#define LEVEL_MEMSIZE_X 100
#define LEVEL_MEMSIZE_Y 100

typedef struct Level {
    const int width;
    const int height;
    const int tiles[LEVEL_MEMSIZE_Y][LEVEL_MEMSIZE_X];
} Level;

#endif
