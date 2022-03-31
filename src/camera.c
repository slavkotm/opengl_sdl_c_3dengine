#include "../header/camera.h"

struct camera
{
    gsl_vector *position;
    gsl_vector *front;
    gsl_vector *up;
    gsl_vector *right;
    gsl_vector *world_up;
    
    double yaw;
    double pitch;

    double fov;
    double znear;
    double zfar;
    double aspect_ratio;

    double movement_speed;
    double sensitivity;
};

struct camera *camera_malloc() 
{ 
    return (struct camera *)malloc(sizeof(struct camera));
};

void camera_constructor(struct camera *item_camera)
{
    item_camera->position = NULL;
    item_camera->front = NULL;
    item_camera->up = NULL;
    item_camera->right = NULL;
    item_camera->world_up = NULL;

    item_camera->yaw = (double)0;
    item_camera->pitch = (double)0;

    item_camera->fov = (double)0;
    item_camera->znear = (double)0;
    item_camera->zfar = (double)0;
    item_camera->aspect_ratio = (double)0;

    item_camera->movement_speed = (double)0;
    item_camera->sensitivity = (double)0;
};

void camera_constructor_assignment(struct camera *item_camera,
                                   gsl_vector *item_position,
                                   gsl_vector *item_front,
                                   gsl_vector *item_up,
                                   gsl_vector *item_world_up,
                                   double item_yaw,
                                   double item_pitch,
                                   double item_fov,
                                   double item_znear,
                                   double item_zfar,
                                   double item_aspect_ratio,
                                   double item_movement_speed,
                                   double item_sensitivity)
{
    item_camera->position = item_position;

    item_camera->front = item_front;

    item_camera->up = item_up;

    item_camera->world_up = item_world_up;

    item_camera->yaw = item_yaw;

    item_camera->pitch = item_pitch;

    item_camera->fov = item_fov;

    item_camera->znear = item_znear;

    item_camera->zfar = item_zfar;

    item_camera->aspect_ratio = item_aspect_ratio;

    item_camera->movement_speed = item_movement_speed;

    item_camera->sensitivity = item_sensitivity;

    camera_update_vectors(item_camera);
};

gsl_matrix *camera_get_view_matrix(struct camera *item_camera)
{
    return matrix_look_at(item_camera->position, 
                          vector_sum(item_camera->position, 
                                     item_camera->front, 3),
                          item_camera->up);
};

gsl_matrix *camera_get_projection_matrix(struct camera *item_camera)
{
    return matrix_perspective(camera_radians(item_camera->fov),
                              item_camera->aspect_ratio,
                              item_camera->znear,
                              item_camera->zfar);
};

gsl_matrix *camera_get_model_matrix() { return matrix_model(); };

void camera_move(struct camera *item_camera, 
                 int32_t dirs)
{
    double velocity = item_camera->movement_speed;

    gsl_vector *direction = gsl_vector_alloc(3);

    gsl_vector_set_zero(direction);

    bool flag = false;

    gsl_vector_set(direction, 2, (double)((dirs & CAMERA_FORWARD)) / CAMERA_FORWARD - (double)((dirs & CAMERA_BACKWARD)) / CAMERA_BACKWARD);

    gsl_vector_set(direction, 0, (double)((dirs & CAMERA_RIGHT)) / CAMERA_RIGHT - (double)((dirs & CAMERA_LEFT)) / CAMERA_LEFT);

    gsl_vector_set(direction, 1, (double)((dirs & CAMERA_UP)) / CAMERA_UP - (double)((dirs & CAMERA_DOWN)) / CAMERA_DOWN);

    for(int i = 0; i < 3; i++)
    {
        if(gsl_vector_get(direction, i) != (double)0)
        {
            flag = true;
            break;
        }
    }

    if(flag)
        direction = vector_normalize(direction, 3);

    for(int i = 0; i < 3; i++)
        gsl_vector_set(item_camera->position, i, gsl_vector_get(item_camera->position, i) + 
                                                 gsl_vector_get(item_camera->front, i) * velocity * gsl_vector_get(direction, 2));

    for(int i = 0; i < 3; i++)
        gsl_vector_set(item_camera->position, i, gsl_vector_get(item_camera->position, i) + 
                                                 gsl_vector_get(item_camera->right, i) * velocity * gsl_vector_get(direction, 0));

    for(int i = 0; i < 3; i++)
        gsl_vector_set(item_camera->position, i, gsl_vector_get(item_camera->position, i) + 
                                                 gsl_vector_get(item_camera->up, i) * velocity * gsl_vector_get(direction, 1));

    camera_update_vectors(item_camera);
};

void camera_rotate(struct camera *item_camera,
                   double x_off_set,
                   double y_off_set)
{
    x_off_set *= item_camera->sensitivity;
    y_off_set *= item_camera->sensitivity;

    item_camera->yaw += x_off_set;
    item_camera->pitch += y_off_set;

    if(item_camera->pitch > (double)89)
        item_camera->pitch = (double)89;

    if(item_camera->pitch < (double)-89)
        item_camera->pitch = (double)-89;

    camera_update_vectors(item_camera);
};

void camera_change_fov(struct camera *item_camera, 
                       double value)
{
    item_camera->fov -= value;

    if(item_camera->fov < (double)1)
        item_camera->fov = (double)1;

    if(item_camera->fov > (double)120)
        item_camera->fov = (double)120;

    camera_update_vectors(item_camera);
};

void camera_update_vectors(struct camera *item_camera)
{
    gsl_vector *item_front = gsl_vector_alloc(3);

    gsl_vector_set(item_front, 0, cos(camera_radians(item_camera->yaw)) * 
                                  cos(camera_radians(item_camera->pitch)));

    gsl_vector_set(item_front, 1, sin(camera_radians(item_camera->pitch)));

    gsl_vector_set(item_front, 2, sin(camera_radians(item_camera->yaw)) * 
                                  cos(camera_radians(item_camera->pitch)));

    item_camera->front = vector_normalize(item_front, 3);

    item_camera->right = vector_normalize(vector_cross(item_camera->front, item_camera->world_up), 3);

    item_camera->up = vector_normalize(vector_cross(item_camera->right, 
                                                    item_camera->front), 3);
};

double camera_radians(double degree) { return (degree * M_PI) / (double)180; };

gsl_vector *camera_get_vector_front(struct camera *item_camera)
{
    return item_camera->front;
};
