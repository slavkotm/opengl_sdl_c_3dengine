#ifndef WINDOW_INCLUDED
#define WINDOW_INCLUDED
#include "../include/SDL2/include/SDL.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define WINDOW_NAME "OpenGL + C + SDL"
#define WINDOW_X_POSITION_SCREEN SDL_WINDOWPOS_CENTERED
#define WINDOW_Y_POSITION_SCREEN SDL_WINDOWPOS_CENTERED

SDL_Window *window_create(const char *name_window, int x_position_screen, int y_position_screen, int width_screen, int height_screen, int flags);
void window_destroy(SDL_Window *window);

#endif
