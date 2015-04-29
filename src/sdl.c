#include "sdl.h"

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct image {
    SDL_Texture* texture;
};

TTF_Font* font = NULL;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

TTF_Font* loadfont(char* file, int ptsize)
{
    TTF_Font* tmpfont;
    tmpfont = TTF_OpenFont(file, ptsize);
    if (tmpfont == NULL) {
        fprintf(
            stderr, "Unable to load font: %s %s\n", file, TTF_GetError());
        exit(1);
    }
    return tmpfont;
}

void text_draw(TTF_Font* fonttodraw,
               Uint8 fgR,
               Uint8 fgG,
               Uint8 fgB,
               Uint8 fgA,
               Uint8 bgR,
               Uint8 bgG,
               Uint8 bgB,
               Uint8 bgA,
               char text[],
               textquality quality,
               SDL_Rect* text_position)
{
    SDL_Color tmpfontcolor = { fgR, fgG, fgB, fgA };
    SDL_Color tmpfontbgcolor = { bgR, bgG, bgB, bgA };
    SDL_Surface* resulting_text = NULL;

    if (quality == solid) {
        resulting_text
            = TTF_RenderText_Solid(fonttodraw, text, tmpfontcolor);
    }
    else if (quality == shaded) {
        resulting_text = TTF_RenderText_Shaded(
            fonttodraw, text, tmpfontcolor, tmpfontbgcolor);
    }
    else if (quality == blended) {
        resulting_text
            = TTF_RenderText_Blended(fonttodraw, text, tmpfontcolor);
    }

    SDL_Texture* textTexture
        = SDL_CreateTextureFromSurface(renderer, resulting_text);

    SDL_Rect dstrect;
    dstrect.x = text_position->x;
    dstrect.y = text_position->y;
    dstrect.w = resulting_text->w;
    dstrect.h = resulting_text->h;

    SDL_RenderCopy(renderer, textTexture, 0, &dstrect);

    SDL_FreeSurface(resulting_text);
    SDL_DestroyTexture(textTexture);
}

struct image* image_new(const char* filename)
{
    struct image* image = (struct image*)malloc(sizeof(struct image));
    image->texture = IMG_LoadTexture(renderer, filename);
    if (!image->texture) {
        fprintf(stderr,
                "Error loading image %s: %s\n",
                filename,
                IMG_GetError());
        exit(1);
    }
    return image;
}

void image_free(struct image* image)
{
    SDL_DestroyTexture(image->texture);
    free(image);
}

void image_dimensions(const struct image* image, int* width, int* height)
{
    Uint32 format;
    int access;
    SDL_QueryTexture(image->texture, &format, &access, width, height);
}

void image_draw(const struct image* image,
                const SDL_Rect* srcrect,
                const SDL_Rect* dstrect)
{
    SDL_RenderCopy(renderer, image->texture, srcrect, dstrect);
}

void sdl_setup(int width, int height)
{
    int error = SDL_Init(SDL_INIT_EVERYTHING);

    if (error) {
        fprintf(stderr, "Failure to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    IMG_Init(IMG_INIT_PNG);

    error = SDL_CreateWindowAndRenderer(
        width, height, SDL_WINDOW_OPENGL, &window, &renderer);

    if (error) {
        fprintf(stderr,
                "Failure to initialize window/renderer: %s\n",
                SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1) {
        fprintf(stderr, "Failure to initialize TTF: %s\n", SDL_GetError());
        exit(1);
    }

    font = TTF_OpenFont("DejaVuSans.ttf", 16);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void sdl_finish()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void sdl_update_screen(void) { SDL_RenderPresent(renderer); }

void sdl_change_color(int r, int g, int b)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void sdl_draw_rect(const SDL_Rect* rect, TTF_Font* font, char* name)
{
    SDL_Rect pilot_name_pos;
    pilot_name_pos.x = rect->x;
    pilot_name_pos.y = rect->y - 10;
    pilot_name_pos.w = 0;
    pilot_name_pos.h = 0;

    text_draw(font,
              255,
              255,
              255,
              70,
              0,
              0,
              0,
              0,
              name,
              blended,
              &pilot_name_pos);

    SDL_RenderFillRect(renderer, rect);
}
