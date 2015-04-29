#ifndef KART_CURVE_H_
#define KART_CURVE_H_

#include "point.h"
struct vector;

struct curve {
    struct point p0, p1, p2, p3;
    struct vector* lengths;
};

void curve_init(struct curve* curve,
                const struct point* p0,
                const struct point* p1,
                const struct point* p2,
                const struct point* p3);
void curve_destroy(struct curve* curve);

double curve_length(const struct curve* curve);
struct point curve_position_at_length(const struct curve* curve,
                                      double length);

#endif
