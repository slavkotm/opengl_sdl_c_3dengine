#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED
#include "../include/SDL2/SDL.h"

#define MAJOR_VERSION 3
#define MINOR_VERSION 3

void context_set(int major_version, int minor_version);
SDL_GLContext context_create(SDL_Window *window);
void context_delete(SDL_GLContext *gl_context);

#endif
