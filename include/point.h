#ifndef KART_POINT_H_
#define KART_POINT_H_

struct point {
    double x, y;
};

void point_init(struct point* point, double x, double y);
void point_destroy(struct point* point);

struct point point_product(double s, const struct point* p);
struct point point_sum(const struct point* p0, const struct point* p1);
struct point point_interpolate(const struct point* p0,
                               const struct point* p1);
double point_distance(const struct point* p0, const struct point* p1);

#endif
