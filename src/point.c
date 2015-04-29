#include "point.h"

#include <math.h>

void point_init(struct point* point, double x, double y)
{
    point->x = x;
    point->y = y;
}

void point_destroy(struct point* point) { point = point; }

struct point point_product(double s, const struct point* p)
{
    struct point result;
    result.x = s * p->x;
    result.y = s * p->y;
    return result;
}

struct point point_sum(const struct point* p0, const struct point* p1)
{
    struct point result;
    result.x = p0->x + p1->x;
    result.y = p0->y + p1->y;
    return result;
}

struct point point_interpolate(const struct point* p0,
                               const struct point* p1)
{
    struct point result;
    result.x = (p0->x + p1->x) / 2;
    result.y = (p0->y + p1->y) / 2;
    return result;
}

double point_distance(const struct point* p0, const struct point* p1)
{
    return sqrt(pow(p1->x - p0->x, 2) + pow(p1->y - p0->y, 2));
}
