#ifndef KART_RUNNER_H_
#define KART_RUNNER_H_

#include "point.h"

struct vector;
struct path;

struct runner {
    struct vector* laptimes;
    int cur_lap;
    double cur_time;
    struct point previous;
    int color[3];
    char name[15];
};

void runner_init(struct runner* runner,
                 const double* laptimes,
                 int nlaptimes,
                 const int* color,
                 const struct point* start_position,
                 const char* name);
void runner_destroy(struct runner* runner);

void runner_move(struct runner* runner, double elapsed_time);
struct point runner_position(const struct runner* runner,
                             const struct path* path);

#endif
