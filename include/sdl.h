#ifndef KART_SDL_H_
#define KART_SDL_H_

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_ttf.h>

struct image;
enum textquality { solid, shaded, blended };

TTF_Font* loadfont(const char* file, int ptsize);
void text_draw(TTF_Font* fonttodraw,
               Uint8 fgR,
               Uint8 fgG,
               Uint8 fgB,
               Uint8 fgA,
               Uint8 bgR,
               Uint8 bgG,
               Uint8 bgB,
               Uint8 bgA,
               const char* text,
               enum textquality quality,
               SDL_Rect* text_position);

struct image* image_new(const char* filename);
void image_free(struct image* image);

void image_dimensions(const struct image* image, int* width, int* height);

void image_draw(const struct image* image,
                const SDL_Rect* srcrect,
                const SDL_Rect* dstrect);

void sdl_setup(int width, int height);
void sdl_finish(void);

void sdl_update_screen(void);
void sdl_change_color(int r, int g, int b);
void sdl_draw_rect(const SDL_Rect* rect, TTF_Font* font, char* name);

#endif
