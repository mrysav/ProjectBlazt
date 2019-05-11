#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdbool.h>

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

bool intersects(RectangleF* rect1, RectangleF* rect2);

#endif
