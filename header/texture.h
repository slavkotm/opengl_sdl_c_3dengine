#ifndef TETRIS_INCLUDED_TEXTURE
#define TETRIS_INCLUDED_TEXTURE
#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"
#include "../include/SDL2/SDL_ttf.h"

SDL_Texture *create_texture(const char *path,
                            SDL_Surface *surface,
                            SDL_Texture *texture,
                            SDL_Renderer *renderer);

SDL_Texture *create_color_texture(const char *path,
                                  SDL_Surface *surface,
                                  SDL_Texture *texture,
                                  SDL_Renderer *renderer,
                                  int red,
                                  int green,
                                  int blue);

void create_text_texture(const char *path,
                                 int font_size,
                                 SDL_Surface *surface,
                                 SDL_Renderer *renderer,
                                 TTF_Font *font,
                                 int red,
                                 int green,
                                 int blue,
                                 int alpha,
                                 const char *text,
                                 SDL_Texture **texture,
                                 SDL_Rect *texture_rectangle);

#endif
