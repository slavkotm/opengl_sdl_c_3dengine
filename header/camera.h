#ifndef CAMERA_INCLUDED
#define CAMERA_INCLUDED
#include "../header/matrix.h"
#include "../header/vector.h"
#include "../include/GL/gl.h"
#include <stdbool.h>

#define YAW             90.0
#define PITCH           0.0
#define SPEED           0.05
#define SENSITIVITY     0.05
#define FOV             45.0
#define ZNEAR           0.01
#define ZFAR            1000.0
#define ASPECTRATIO     16.0 / 9.0


enum camera_movement 
{
    CAMERA_FORWARD  = 1,
    CAMERA_BACKWARD = 2,
    CAMERA_RIGHT    = 4,
    CAMERA_LEFT     = 8,
    CAMERA_UP       = 16,
    CAMERA_DOWN     = 32
};

struct camera;

void camera_constructor_init(struct camera *);

void camera_constructor_assignment(struct camera *,
                                   gsl_vector *,
                                   gsl_vector *,
                                   gsl_vector *,
                                   gsl_vector *,
                                   double,
                                   double,
                                   double,
                                   double,
                                   double,
                                   double,
                                   double,
                                   double);

gsl_matrix *camera_get_view_matrix(struct camera *);

gsl_matrix *camera_get_model_matrix();

gsl_matrix *camera_get_projection_matrix(struct camera *);

void camera_move(struct camera *, 
                 int32_t);

void camera_rotate(struct camera *,
                   double,
                   double);

void camera_change_fov(struct camera *,
                       double);

double camera_radians(double);

void camera_update_vectors(struct camera *);

void camera_destroy(struct camera *);

struct camera *camera_malloc();

gsl_vector *camera_get_vector_front(struct camera *);

void camera_set_polygon_mode(bool);

#endif
