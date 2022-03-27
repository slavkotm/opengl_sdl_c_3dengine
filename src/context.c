#include "../header/context.h"

void context_set(int major_version,
                 int minor_version)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 
                        major_version);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 
                        minor_version);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
}

void context_delete(SDL_GLContext *gl_context)
{
    SDL_GL_DeleteContext(*gl_context);
}

SDL_GLContext context_create(SDL_Window *window) 
{ 
    return SDL_GL_CreateContext(window);
}
