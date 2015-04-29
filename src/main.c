#include <unistd.h>
#include <sys/time.h>

#include "vector.h"
#include "path.h"
#include "runner.h"
#include "sdl.h"
#include "data.h"

static const int screen_width = 767;
static const int screen_height = 432;

static struct path build_path(void)
{
    struct path path;
    path_init_with_coords(&path, coordinates, ncoordinates);
    return path;
}

static struct vector* build_runners(const struct path* path)
{
    int i;
    struct vector* runners = vector_new(sizeof(struct runner), nrunners);
    const struct point start_position = path_position_at_length(path, 0);
    for (i = 0; i < nrunners; ++i) {
        runner_init((struct runner*)vector_grow(&runners),
                    laptimes + i * nlaps,
                    nlaps,
                    colors + i * 3,
                    &start_position,
                    names[i]);
    }
    return runners;
}

static void handle_events(int* done, int* running, int* multiplier)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            *done = 1;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_SPACE) {
                *running = !(*running);
            }
            else if (event.key.keysym.sym == SDLK_1) {
                *multiplier = 1;
            }
            else if (event.key.keysym.sym == SDLK_2) {
                *multiplier = 2;
            }
            else if (event.key.keysym.sym == SDLK_3) {
                *multiplier = 3;
            }
            break;
        }
    }
}

static void draw(struct vector* runners,
                 const struct path* path,
                 const struct image* background,
                 TTF_Font* font,
                 char* time_text)
{
    int i;
    SDL_Rect rect;
    static const int width = 10;
    static const int height = 10;

    for (i = 0; i < nrunners; ++i) {
        const struct runner* runner
            = (const struct runner*)vector_get(runners, i);
        SDL_Rect old_rect;
        old_rect.x = runner->previous.x - width / 2;
        old_rect.y = runner->previous.y - (height / 2) - 10;
        old_rect.w = width + 70;
        old_rect.h = height + 30;
        if (old_rect.x + old_rect.w > screen_width) {
            old_rect.w = screen_width - old_rect.x;
        }
        else if (old_rect.y + old_rect.y > screen_height) {
            old_rect.h = screen_height - old_rect.y;
        }
        image_draw(background, &old_rect, &old_rect);
    }

    for (i = 0; i < nrunners; ++i) {
        struct runner* runner = (struct runner*)vector_get(runners, i);
        const struct point new_position = runner_position(runner, path);
        SDL_Rect new_rect;
        new_rect.x = new_position.x - width / 2;
        new_rect.y = new_position.y - height / 2;
        new_rect.w = width;
        new_rect.h = height;

        sdl_change_color(
            runner->color[0], runner->color[1], runner->color[2]);
        sdl_draw_rect(&new_rect, font, runner->name);
        runner->previous = new_position;
    }

    rect.x = 700;
    rect.y = 380;
    rect.w = 50;
    rect.h = 10;

    image_draw(background, &rect, &rect);
    text_draw(
        font, 255, 255, 255, 70, 0, 0, 0, 0, time_text, blended, &rect);
}

static void show_background(const struct image* background)
{
    image_draw(background, 0, 0);
}

#define timersub(a, b, result)                                            \
    do {                                                                  \
        (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;                     \
        (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;                  \
        if ((result)->tv_usec < 0) {                                      \
            --(result)->tv_sec;                                           \
            (result)->tv_usec += 1000000;                                 \
        }                                                                 \
    } while (0)

static double calculate_elapsed_time(struct timeval* start_time)
{
    struct timeval diff, current_time;
    gettimeofday(&current_time, 0);
    timersub(&current_time, start_time, &diff);
    *start_time = current_time;
    return diff.tv_sec * 1000.0 + diff.tv_usec / 1000.0;
}

static void update(struct vector* runners, double elapsed_time)
{
    int i;
    for (i = 0; i < nrunners; ++i) {
        struct runner* runner = (struct runner*)vector_get(runners, i);
        runner_move(runner, elapsed_time);
    }
}

int snprintf(char* str, size_t size, const char* format, ...);

static void loop(const struct path* path,
                 struct vector* runners,
                 const struct image* background)
{
    int done = 0;
    int running = 0;
    int multiplier = 1;
    double total_race_time = 0;
    struct timeval start_time;
    const char* fontName = "DejaVuSans.ttf";
    TTF_Font* font = loadfont(fontName, 10);

    gettimeofday(&start_time, 0);
    show_background(background);

    while (!done) {
        char s_total_time[10];
        int race_time_min;
        int race_time_sec;
        int race_time_msec;

        double elapsed_time = calculate_elapsed_time(&start_time);
        handle_events(&done, &running, &multiplier);
        if (running) {
            elapsed_time = elapsed_time * multiplier;
            update(runners, elapsed_time);
            total_race_time += elapsed_time;
        }

        race_time_min = (int)total_race_time / (1000 * 60);
        race_time_sec = (int)(total_race_time / (1000)) % 60;
        race_time_msec = total_race_time - (race_time_min * (60 * 1000))
            - (race_time_sec * 1000);
        snprintf(s_total_time,
                 10,
                 "%02d:%02d:%03d",
                 race_time_min,
                 race_time_sec,
                 race_time_msec);
        draw(runners, path, background, font, s_total_time);
        sdl_update_screen();
    }
}

int main()
{
    struct path path = build_path();
    struct vector* runners = build_runners(&path);
    struct image* background;

    sdl_setup(767, 432);

    background = image_new("sanmarino12counter.png");

    loop(&path, runners, background);

    image_free(background);

    sdl_finish();

    VECTOR_FOREACH(runners, runner_destroy);
    vector_free(runners);
    path_destroy(&path);

    return 0;
}
