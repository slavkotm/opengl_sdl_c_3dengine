#include "../header/event.h"

struct event
{
    SDL_Event *event;
    bool running;
};

struct event *event_malloc() 
{ 
    return (struct event *)malloc(sizeof(struct event)); 
};

void event_init(struct event *item_event)
{
    item_event->running = true;
}

void event_destroy(struct event *item_event) { free(item_event); };

void event_handle(struct event *item_event)
{
    while(SDL_PollEvent(item_event->event))
    {

        if(item_event->event->type == SDL_QUIT)
        {
            item_event->running = false;
        }
        
        if(item_event->event->type == SDL_KEYDOWN)
        {
            if(item_event->event->key.keysym.sym == SDLK_ESCAPE)
            {
                item_event->running = false;
            }
        }
    }
};
