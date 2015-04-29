#include "path.h"

#include <assert.h>

#include "vector.h"
#include "point.h"
#include "curve.h"
#include "utils.h"

void path_init_with_coords(struct path* path,
                           const struct point* coords,
                           int ncoords)
{
    double total_length = 0;
    int i, ncurves = (ncoords - 1) / 3;
    assert((ncoords + 2) % 3 == 0);
    path->curves = vector_new(sizeof(struct curve), ncurves);
    path->lengths = vector_new(sizeof(double), ncurves);
    for (i = 0; i < ncurves; ++i) {
        struct curve* new_curve
            = (struct curve*)vector_grow(&path->curves);
        curve_init(new_curve,
                   coords + i * 3 + 0,
                   coords + i * 3 + 1,
                   coords + i * 3 + 2,
                   coords + i * 3 + 3);
        total_length += curve_length(new_curve);
        vector_push_back(&path->lengths, &total_length);
    }
}

void path_destroy(struct path* path)
{
    VECTOR_FOREACH(path->curves, curve_destroy);
    vector_free(path->curves);
    vector_free(path->lengths);
}

double path_length(const struct path* path)
{
    return *((double*)vector_last(path->lengths));
}

struct point path_position_at_length(const struct path* path,
                                     double length)
{
    const int i = VECTOR_BSEARCH(path->lengths, &length, double_compare);
    const double found = *((double*)vector_get(path->lengths, i));
    const struct curve* curve
        = (const struct curve*)vector_get(path->curves, i + 1);
    return curve_position_at_length(curve, length - found);
}
