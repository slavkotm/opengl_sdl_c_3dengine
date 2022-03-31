#ifndef EVENT_INCLUDED
#define EVENT_INCLUDED
#include "../include/SDL2/SDL.h"
#include "../header/camera.h"
#include <stdbool.h>
#include <stdlib.h>

struct event;

struct event *event_malloc();

void event_init(struct event *,
                struct camera *,
                bool,
                int32_t,
                double, 
                double,
                double,
                double,
                double,
                double,
                bool);

void event_destroy(struct event *);

void event_handle(struct event *);

bool event_get_running(struct event *);

int32_t event_get_dir(struct event *);

double event_get_x_off_set(struct event *);
double event_get_y_off_set(struct event *);

bool event_get_space(struct event *);

#endif
