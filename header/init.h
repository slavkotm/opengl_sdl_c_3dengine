#ifndef INIT_INCLUDED
#define INIT_INCLUDED
#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_mixer.h"
#include <stdbool.h>

#define RENDER_INIT true
#define AUDIO_INIT false

bool init(bool render_init, bool audio_init);
void quit();

#endif
