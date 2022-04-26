#include "../header/render.h"

void render_background(SDL_Renderer *renderer, 
                       SDL_Texture *texture, 
                       SDL_Rect source_rectangle, 
                       SDL_Rect destination_rectangle)
{
    SDL_RenderCopy(renderer,
                   texture,
                   &source_rectangle,
                   &destination_rectangle);
}

void size_background(SDL_Rect *source_rectangle, 
                     SDL_Rect *destination_rectangle,
                     int width,
                     int height)
{
    source_rectangle->x = source_rectangle->y = 0;
    destination_rectangle->x = destination_rectangle->y = 0;

    source_rectangle->w = (int)width;
    source_rectangle->h = (int)height;

    destination_rectangle->w = (int)width;
    destination_rectangle->h = (int)height;
}

void render_text(SDL_Renderer *renderer, 
                 SDL_Texture *texture, 
                 SDL_Rect rectangle)
{
    SDL_RenderCopy(renderer, texture, NULL, &rectangle);
}

void position_text(SDL_Rect *rectangle, 
                   int position_x, 
                   int position_y,
                   int move_x,
                   int move_y)
{
    rectangle->x = (position_x - rectangle->w) / 2 + move_x;
    rectangle->y = (position_y - rectangle->h) / 2 + move_y;
}
