#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>
#include <stdint.h>

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct VectorF {
    float x;
    float y;
} VectorF;

typedef struct RectangleF {
    float x;
    float y;
    float width;
    float height;
} RectangleF;

typedef struct Rect32 {
    int_fast32_t x;
    int_fast32_t y;
    int_fast32_t width;
    int_fast32_t height;
} Rect32;

bool intersects(RectangleF* rect1, RectangleF* rect2);

#endif
