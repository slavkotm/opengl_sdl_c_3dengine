#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED
#include "../include/SDL2/SDL.h"

void context_set(int ,
                 int );

SDL_GLContext context_create(SDL_Window *);

void context_delete(SDL_GLContext *);

#endif
