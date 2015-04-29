#include "runner.h"

#include <string.h>

#include "vector.h"
#include "path.h"
#include "utils.h"

void runner_init(struct runner* runner,
                 const double* laptimes,
                 int nlaptimes,
                 const int* color,
                 const struct point* start_position,
                 const char* name)
{
    runner->laptimes = vector_new(sizeof(double), nlaptimes);
    vector_memcpy(&runner->laptimes, laptimes, nlaptimes);
    runner->cur_lap = 0;
    runner->cur_time = 0;
    runner->previous = *start_position;
    memcpy(runner->color, color, sizeof(int) * 3);
    memcpy(runner->name, name, 14);
}

void runner_destroy(struct runner* runner)
{
    vector_free(runner->laptimes);
}

static void runner_validate_cur_time(struct runner* runner)
{
    if (runner->cur_lap == vector_size(runner->laptimes)) {
        runner->cur_time = 0;
    }
    else {
        const double laptime
            = *((double*)vector_get(runner->laptimes, runner->cur_lap));

        if (double_compare(&runner->cur_time, &laptime) > 0) {
            runner->cur_lap += 1;
            runner->cur_time -= laptime;
            runner_validate_cur_time(runner);
        }
    }
}

void runner_move(struct runner* runner, double elapsed_time)
{
    runner->cur_time += elapsed_time;
    runner_validate_cur_time(runner);
}

struct point runner_position(const struct runner* runner,
                             const struct path* path)
{
    const double length = path_length(path);
    const double laptime
        = *((double*)vector_get(runner->laptimes, runner->cur_lap));
    const double distancia_percorrida = length * runner->cur_time
        / laptime;
    return path_position_at_length(path, distancia_percorrida);
}
