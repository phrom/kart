#ifndef KART_PATH_H_
#define KART_PATH_H_

struct vector;
struct point;

struct path {
    struct vector* curves;
    struct vector* lengths;
};

void path_init_with_coords(struct path* path,
                           const struct point* coords,
                           int ncoords);
void path_destroy(struct path* path);

double path_length(const struct path* path);
struct point path_position_at_length(const struct path* path,
                                     double length);

#endif
