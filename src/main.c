#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "../include/glad/glad.h"
#include "../include/SDL2/SDL.h" 
#include "../include/GL/gl.h"
#include "../include/gsl/gsl_blas.h"
#include "../include/gsl/gsl_matrix_double.h"
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
               0,
               false,
               false);



    gsl_matrix *model = camera_get_model_matrix();
    gsl_matrix *model1 = camera_get_model_matrix();
    gsl_matrix *model2 = camera_get_model_matrix();

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
    double move = 0;
    double move1 = 0;

    glad_glEnable(GL_DEPTH_TEST);  
    glad_glFrontFace(GL_CW);

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(window, 640, 360);

    while(event_get_running(item_event))
    {
        event_handle(item_event);
        camera_move(item_camera, event_get_dir(item_event));
        camera_rotate(item_camera, event_get_x_off_set(item_event), -event_get_y_off_set(item_event));
        camera_set_polygon_mode(event_get_space(item_event));
        camera_set_cull_face_mode(event_get_cull_face(item_event));
                
        glad_glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gsl_matrix *pv = gsl_matrix_alloc(4, 4);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, (double)1, camera_get_projection_matrix(item_camera), camera_get_view_matrix(item_camera), 0, pv);

        GLfloat a[16]; 
        matrix_to_array(camera_get_view_matrix(item_camera), a, 4, 4);
        
        GLfloat p[16];
        matrix_to_array(camera_get_projection_matrix(item_camera), p, 4, 4);

        GLfloat m[16];
        matrix_to_array(model, m, 4, 4);

        shader_use(shaders);
        GLuint transformLoc = glad_glGetUniformLocation(get_id(shaders), "model");
        glad_glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const GLfloat *)m);
        GLuint view = glad_glGetUniformLocation(get_id(shaders), "view");
        glad_glUniformMatrix4fv(view, 1, GL_FALSE,(const GLfloat *)a);
        GLuint projection = glad_glGetUniformLocation(get_id(shaders), "projection");
        glad_glUniformMatrix4fv(projection, 1, GL_FALSE, (const GLfloat *)p);

        matrix_set_value(model, 1, 1, cos(PI * angle / 180));
        matrix_set_value(model, 2, 1, sin(PI * angle / 180));
        matrix_set_value(model, 1, 2,-sin(PI * angle / 180));
        matrix_set_value(model, 2, 2, cos(PI * angle / 180));
        angle += 1.0;

        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



        GLfloat mm[16];
        matrix_to_array(model1, mm, 4, 4);

        GLuint transformLoc1 = glad_glGetUniformLocation(get_id(shaders), "model");
        glad_glUniformMatrix4fv(transformLoc1, 1, GL_FALSE, (const GLfloat *)mm);
        GLuint view1 = glad_glGetUniformLocation(get_id(shaders), "view");
        glad_glUniformMatrix4fv(view1, 1, GL_FALSE,(const GLfloat *)a);
        GLuint projection1 = glad_glGetUniformLocation(get_id(shaders), "projection");
        glad_glUniformMatrix4fv(projection1, 1, GL_FALSE, (const GLfloat *)p);

        matrix_set_value(model1, 3, 0, move1);
        matrix_set_value(model1, 3, 1, move1);
        move1 += 0.001;

//        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);





        GLfloat mmm[16];
        matrix_to_array(model2, mmm, 4, 4);

        GLuint transformLoc2 = glad_glGetUniformLocation(get_id(shaders), "model");
        glad_glUniformMatrix4fv(transformLoc2, 1, GL_FALSE, (const GLfloat *)mmm);
        GLuint view2 = glad_glGetUniformLocation(get_id(shaders), "view");
        glad_glUniformMatrix4fv(view2, 1, GL_FALSE,(const GLfloat *)a);
        GLuint projection2 = glad_glGetUniformLocation(get_id(shaders), "projection");
        glad_glUniformMatrix4fv(projection2, 1, GL_FALSE, (const GLfloat *)p);

        matrix_set_value(model2, 3, 0, move);
        matrix_set_value(model2, 3, 1, move);
        matrix_set_value(model2, 3, 2, move);
        move += 0.01;
//        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    event_destroy(item_event);
    shader_destroy(shaders);
    camera_destroy(item_camera);
    free(item_camera);

    glad_glDeleteVertexArrays(1, &VAO);
    glad_glDeleteBuffers(1, &VBO); 
    glad_glDeleteBuffers(1, &EBO); 

    context_delete(&gl_context);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
