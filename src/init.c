#include "../header/init.h"

bool init(bool render_init, bool audio_init)
{
    bool flag = true;

    if(render_init)
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) 
        {
            printf("Unable to initialize SDL: %s\n", SDL_GetError());
            flag = false;
        }
    }

    if(audio_init)
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) 
        {
            printf("Mix_OpenAudio: %s\n", Mix_GetError());
            flag = false;
        }
    }

    return flag;
}

void quit() { SDL_Quit(); };
