#include "../include/glad/glad.h"
#include "../include/SDL2/SDL.h" 
#include "../include/GL/gl.h"
#include "../include/gsl/gsl_blas.h"
#include "../include/gsl/gsl_matrix_double.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "../header/read.h"
#include "../header/shader.h"
#include "../header/init.h"
#include "../header/window.h"
#include "../header/context.h"
#include "../header/event.h"
#include "../header/vector.h"
#include "../header/camera.h"
#define PI 3.141592654

int main(int argc, 
         char **argv)
{
    init(true, false);

    SDL_Window *window = window_create("OpenGL + C + SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    
    SDL_GLContext gl_context = context_create(window);
    context_set(3, 3);

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    gsl_vector *vector_position = gsl_vector_alloc(3);
    gsl_vector *vector_front = gsl_vector_alloc(3);
    gsl_vector *vector_up = gsl_vector_alloc(3);
    gsl_vector *vector_world_up = gsl_vector_alloc(3);

    gsl_vector_set(vector_position, 0, (double)0);
    gsl_vector_set(vector_position, 1, (double)0);
    gsl_vector_set(vector_position, 2, (double)-2);

    gsl_vector_set(vector_front, 0, (double)0);
    gsl_vector_set(vector_front, 1, (double)0);
    gsl_vector_set(vector_front, 2, (double)-1);

    gsl_vector_set(vector_up, 0, (double)0);
    gsl_vector_set(vector_up, 1, (double)1);
    gsl_vector_set(vector_up, 2, (double)0);

    vector_world_up = vector_up;

    struct camera *item_camera = camera_malloc();
    camera_constructor_assignment(item_camera,
                                  vector_position,
                                  vector_front,
                                  vector_up,
                                  vector_world_up,
                                  YAW,
                                  PITCH,
                                  FOV,
                                  ZNEAR,
                                  ZFAR,
                                  ASPECTRATIO,
                                  SPEED,
                                  SENSITIVITY);

    struct event *item_event = event_malloc();
    event_init(item_event, 
               item_camera,
               true,
               0,
               0,
               0,
               0,
               0,
               0,
               0);


    gsl_vector *a = gsl_vector_alloc(3);
    gsl_vector_set(a, 0, 0);
    gsl_vector_set(a, 1, 0);
    gsl_vector_set(a, 2, 3);

    gsl_vector *bbb = gsl_vector_alloc(3);
    gsl_vector_set(bbb, 0, 0);
    gsl_vector_set(bbb, 1, 0);
    gsl_vector_set(bbb, 2, 0);

    gsl_vector *c = gsl_vector_alloc(3);
    gsl_vector_set(c, 0, 0);
    gsl_vector_set(c, 1, 1);
    gsl_vector_set(c, 2, 0);

    struct shader *shaders = shader_malloc();

    shader_init(shaders, "shaders/basic.vert", "shaders/basic.frag", "r");


    GLfloat vertex[] = {
       -0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,    0.5f, 0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
        0, 4, 1,
        1, 4, 5,
        0, 3, 7,
        0, 7, 4,
        1, 6, 2,
        1, 5, 6,
        2, 7, 3,
        2, 6, 7,
        4, 7, 5,
        5, 7, 6
    };

    GLuint VBO, VAO, EBO;
    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);
    glad_glGenBuffers(1, &EBO);

    glad_glBindVertexArray(VAO);

    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
    glad_glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glad_glEnableVertexAttribArray(1);

    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);

    glad_glBindVertexArray(0);

    double angle = 45.0;

    glad_glEnable(GL_DEPTH_TEST);  
    /*glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
    GLfloat r = 0.2f, g = 0.3f, b = 0.3f;

    float **mmp = malloc(4 * sizeof(float*));

    for(int i = 0; i < 4; i++)
        mmp[i] = malloc(4 * sizeof(float));


    float **mmv = malloc(4 * sizeof(float*));

    for(int i = 0; i < 4; i++)
        mmv[i] = malloc(4 * sizeof(float));

    float **mmm = malloc(4 * sizeof(float*));

    for(int i = 0; i < 4; i++)
        mmm[i] = malloc(4 * sizeof(float));

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            mmm[i][j] = 0;

    mmm[0][0] = 1;
    mmm[1][1] = 1;
    mmm[2][2] = 1;
    mmm[3][3] = 1;

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(window, 640, 360);

    while(event_get_running(item_event))
    {
        event_handle(item_event);
        camera_move(item_camera, event_get_dir(item_event));
        camera_rotate(item_camera, event_get_x_off_set(item_event), -event_get_y_off_set(item_event));
                
        glad_glClearColor(r, g, b, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gsl_matrix *pv = gsl_matrix_alloc(4, 4);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, (double)1, camera_get_projection_matrix(item_camera), 

        camera_get_view_matrix(item_camera), 0, pv);

        matrix_get_float(camera_get_view_matrix(item_camera), (float **)mmv, 4, 4);
        matrix_get_float(camera_get_projection_matrix(item_camera), (float **)mmp, 4, 4);

        GLfloat a[16]; 
        a[0] = mmv[0][0];
        a[1] = mmv[0][1];
        a[2] = mmv[0][2];
        a[3] = mmv[0][3];
        a[4] = mmv[1][0];
        a[5] = mmv[1][1];
        a[6] = mmv[1][2];
        a[7] = mmv[1][3];
        a[8] = mmv[2][0];
        a[9] = mmv[2][1];
        a[10] = mmv[2][2];
        a[11] = mmv[2][3];
        a[12] = mmv[3][0];
        a[13] = mmv[3][1];
        a[14] = mmv[3][2];
        a[15] = mmv[3][3];

        GLfloat ammm[16];
        ammm[0] = mmm[0][0];
        ammm[1] = mmm[0][1];
        ammm[2] = mmm[0][2];
        ammm[3] = mmm[0][3];
        ammm[4] = mmm[1][0];
        ammm[5] = mmm[1][1];
        ammm[6] = mmm[1][2];
        ammm[7] = mmm[1][3];
        ammm[8] = mmm[2][0];
        ammm[9] = mmm[2][1];
        ammm[10] = mmm[2][2];
        ammm[11] = mmm[2][3];
        ammm[12] = mmm[3][0];
        ammm[13] = mmm[3][1];
        ammm[14] = mmm[3][2];
        ammm[15] = mmm[3][3];


        GLfloat p[16];
        p[0] = mmp[0][0];
        p[1] = mmp[0][1];
        p[2] = mmp[0][2];
        p[3] = mmp[0][3];
        p[4] = mmp[1][0];
        p[5] = mmp[1][1];
        p[6] = mmp[1][2];
        p[7] = mmp[1][3];
        p[8] = mmp[2][0];
        p[9] = mmp[2][1];
        p[10] = mmp[2][2];
        p[11] = mmp[2][3];
        p[12] = mmp[3][0];
        p[13] = mmp[3][1];
        p[14] = mmp[3][2];
        p[15] = mmp[3][3];

        shader_use(shaders);
        GLuint transformLoc = glad_glGetUniformLocation(get_id(shaders), "model");
        glad_glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const GLfloat *)ammm);
        GLuint view = glad_glGetUniformLocation(get_id(shaders), "view");
        glad_glUniformMatrix4fv(view, 1, GL_FALSE,(const GLfloat *)a);
        GLuint projection = glad_glGetUniformLocation(get_id(shaders), "projection");
        glad_glUniformMatrix4fv(projection, 1, GL_FALSE, (const GLfloat *)p);

        mmm[1][1] = cos(PI * angle / 180);
        mmm[2][1] = sin(PI * angle / 180);
        mmm[1][2] = -sin(PI * angle / 180);
        mmm[2][2] = cos(PI * angle / 180);
        angle += 1.0;

        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    printf("%s\n", glGetString(GL_VERSION));

    free(shaders);

    glad_glDeleteVertexArrays(1, &VAO);
    glad_glDeleteBuffers(1, &VBO); 
    glad_glDeleteBuffers(1, &EBO); 

    context_delete(&gl_context);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
