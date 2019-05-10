#ifndef __UTILS_H__
#define __UTILS_H__

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
