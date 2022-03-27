#ifndef EVENT_INCLUDED
#define EVENT_INCLUDED
#include "../include/SDL2/SDL.h"
#include <stdbool.h>
#include <stdlib.h>

struct event;

struct event *event_malloc();

void event_init(struct event *);
void event_destroy(struct event *);

void event_handle(struct event *);

#endif
