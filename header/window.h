#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED
#include "../include/SDL2/SDL.h"

SDL_Window *window_create(const char* , 
                          int , 
                          int , 
                          int , 
                          int , 
                          int );

void window_destroy(SDL_Window *);

#endif
