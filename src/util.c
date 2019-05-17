#include "util.h"

bool intersects(RectangleF *rect1, RectangleF *rect2) {
  if (rect1->x > rect2->x || rect2->x > rect1->x) {
    return false;
  }

  if (rect1->y > rect2->y || rect2->y > rect1->y) {
    return false;
  }

  return true;
}
