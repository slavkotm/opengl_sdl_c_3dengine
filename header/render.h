#ifndef TETRIS_INCLUDED_RENDER
#define TETRIS_INCLUDED_RENDER
#include "../include/SDL2/SDL.h"

void render_background(SDL_Renderer *renderer,
                       SDL_Texture *texture,
                       SDL_Rect source_rectangle,
                       SDL_Rect destination_rectangle);

void size_background(SDL_Rect *source_rectangle,
                     SDL_Rect *destination_rectangle,
                     int width,
                     int height);

void render_text(SDL_Renderer *renderer,
                 SDL_Texture *texture,
                 SDL_Rect rectangle);

void position_text(SDL_Rect *rectangle,
                   int position_x,
                   int position_y,
                   int move_x,
                   int move_y);

#endif
