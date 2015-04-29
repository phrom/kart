#include "curve.h"

#include <math.h>

#include "vector.h"
#include "utils.h"

#define BEZIER_LENGTH_PRECISION 100000.0

static struct point cubic_bezier(double t, const struct curve* curve)
{
    const struct point* p0 = &curve->p0;
    const struct point* p1 = &curve->p1;
    const struct point* p2 = &curve->p2;
    const struct point* p3 = &curve->p3;

    const struct point t1 = point_product(pow(1 - t, 3), p0);
    const struct point t2 = point_product(3 * pow(1 - t, 2) * t, p1);
    const struct point t3 = point_product(3 * (1 - t) * pow(t, 2), p2);
    const struct point t4 = point_product(pow(t, 3), p3);

    const struct point s1 = point_sum(&t1, &t2);
    const struct point s2 = point_sum(&s1, &t3);
    const struct point s3 = point_sum(&s2, &t4);

    return s3;
}

static void curve_calculate_length(struct curve* curve)
{
    int i;
    double total = 0;
    struct point last = curve->p0;
    for (i = 1; i <= BEZIER_LENGTH_PRECISION; ++i) {
        const double t = i / BEZIER_LENGTH_PRECISION;
        const struct point p = cubic_bezier(t, curve);
        const double approx_length = point_distance(&p, &last);
        total += approx_length;
        last = p;
        *((double*)vector_grow(&curve->lengths)) = total;
    }
}

void curve_init(struct curve* curve,
                const struct point* p0,
                const struct point* p1,
                const struct point* p2,
                const struct point* p3)
{
    curve->p0 = *p0;
    curve->p1 = *p1;
    curve->p2 = *p2;
    curve->p3 = *p3;
    curve->lengths = vector_new(sizeof(double), BEZIER_LENGTH_PRECISION);
    curve_calculate_length(curve);
}

void curve_destroy(struct curve* curve) { vector_free(curve->lengths); }

double curve_length(const struct curve* curve)
{
    return *((double*)vector_last(curve->lengths));
}

struct point curve_position_at_length(const struct curve* curve,
                                      double length)
{
    const int i = VECTOR_BSEARCH(curve->lengths, &length, double_compare);
    const double found = *((double*)vector_get(curve->lengths, i));
    if (double_compare(&found, &length) == 0) {
        return cubic_bezier((i + 1) / BEZIER_LENGTH_PRECISION, curve);
    }
    else {
        const struct point p0
            = cubic_bezier((i + 1) / BEZIER_LENGTH_PRECISION, curve);
        const struct point p1
            = cubic_bezier((i + 2) / BEZIER_LENGTH_PRECISION, curve);
        return point_interpolate(&p0, &p1);
    }
}
