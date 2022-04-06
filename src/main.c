#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "../include/glad/glad.h"
#include "../include/SDL2/SDL.h" 
#include "../include/GL/gl.h"
#include "../include/gsl/gsl_blas.h"
#include "../include/gsl/gsl_matrix_double.h"
#include "../include/stb/stb_image.h"
#include "../include/assimp/cimport.h"
#include "../include/assimp/scene.h"
#include "../include/assimp/postprocess.h"
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

    struct shader *simple_shader = shader_malloc();
    struct shader *light_shader = shader_malloc();

    shader_init(simple_shader, "shaders/basic.vert", "shaders/basic.frag", "r");
    shader_init(light_shader, "shaders/light.vert", "shaders/light.frag", "r");

    float cube[] = {

	    -1.0f,-1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f,-1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f, 1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
    	-1.0f,-1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f, 1.0f, 1.0f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f, 1.0f,-1.0f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,

	    1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	    -1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	    -1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f, 	1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	    1.0f, 1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
    	1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	    -1.0f,-1.0f,-1.0f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	    1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	    1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	    1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	    -1.0f,-1.0f,-1.0f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,

	    -1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	    -1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	    1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,
	    1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	    -1.0f, 1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,		0.0f, 0.0f, 1.0f,
	    1.0f,-1.0f, 1.0f,	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,		0.0f, 0.0f, 1.0f,

	    1.0f, 1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	    1.0f,-1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	    1.0f, 1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	    1.0f,-1.0f,-1.0f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
	    1.0f, 1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
	    1.0f,-1.0f, 1.0f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,

	    1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	    1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	    1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f, 1.0f,-1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f, 1.0f, 1.0f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f
   };

	GLint box_width, box_height, channels;
	stbi_uc* data = stbi_load("assets/box.png", &box_width, &box_height, &channels, 0);
	GLuint box_texture;
	glGenTextures(1, &box_texture);

	glBindTexture(GL_TEXTURE_2D, box_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGB,  GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	GLuint VBO_polygon, VAO_polygon;
	glad_glGenBuffers(1, &VBO_polygon);
	glad_glGenVertexArrays(1, &VAO_polygon);

	glad_glBindVertexArray(VAO_polygon);
	glad_glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);
	glad_glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)0);
	glad_glEnableVertexAttribArray(0);

	glad_glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glad_glEnableVertexAttribArray(1);

	glad_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glad_glEnableVertexAttribArray(2);
    
	glad_glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
	glad_glEnableVertexAttribArray(3);

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(window, 640, 360);

    float move = 0.01f;
    glad_glEnable(GL_DEPTH_TEST);

    GLfloat mm[16];
    GLfloat mm1[16];
    GLfloat cgp[3];
    gsl_vector *gsl_cgp = gsl_vector_alloc(3);
    while(event_get_running(item_event))
    {
        event_handle(item_event);
        camera_move(item_camera, event_get_dir(item_event));
        camera_rotate(item_camera, event_get_x_off_set(item_event), -event_get_y_off_set(item_event));
        camera_set_cull_face_mode(event_get_cull_face(item_event));
        camera_set_polygon_mode(event_get_space(item_event));
                
        glad_glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        GLfloat a[16]; 
        matrix_to_array(camera_get_view_matrix(item_camera), a, 4, 4);
        
        GLfloat p[16];
        matrix_to_array(camera_get_projection_matrix(item_camera), p, 4, 4);

        matrix_to_array(model, mm, 4, 4);
        //mm[12] = move;

        /*mm[0] = cos(M_PI * move * 50 / 180.0);
        mm[8] = -sin(M_PI * move * 50 / 180.0);
        mm[2] = sin(M_PI * move * 50/ 180.0);
        mm[10] = cos(M_PI * move * 50/ 180.0);*/

        move += 0.01;
       
        gsl_cgp = camera_get_position(item_camera);

        cgp[0] = gsl_vector_get(gsl_cgp, 0);
        cgp[1] = gsl_vector_get(gsl_cgp, 1);
        cgp[2] = gsl_vector_get(gsl_cgp, 2);

        shader_use(simple_shader);
        shader_set_matrix4f(simple_shader, "model", mm);
        shader_set_matrix4f(simple_shader, "view", a);
        shader_set_matrix4f(simple_shader, "projection", p);
        shader_set_bool(simple_shader, "wireframeMode", event_get_space(item_event));
        shader_set_vec3(simple_shader, "lightPos", 0.0f, 5.0f * cos(move * 1.2f), 5.0f * sin(move  *1.2f));
        shader_set_vec3(simple_shader, "lightColor", 1.0f, 1.0f, 1.0f);
        shader_set_vec3(simple_shader, "ambientColor", 1.0f, 1.0f, 1.0f);
        shader_set_vec3(simple_shader, "viewPos", cgp[0], cgp[1], cgp[2]);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        matrix_to_array(model1, mm1, 4, 4);
        mm1[13] = 5.0f * cos(move * 1.2f);
        mm1[14] = 5.0f * sin(move  *1.2f);

        mm1[0] = 0.2f;
        mm1[5] = 0.2f;
        mm1[10] = 0.2f;
        //mm[12] = move;

        shader_use(light_shader);
        shader_set_matrix4f(light_shader, "model", mm1);
        shader_set_matrix4f(light_shader, "view", a);
        shader_set_matrix4f(light_shader, "projection", p);
        shader_set_vec3(light_shader, "lightColor", 1.0f, 1.0f, 1.0f);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        SDL_GL_SwapWindow(window);
    }

    event_destroy(item_event);
    shader_destroy(simple_shader);
    camera_destroy(item_camera);
    free(item_camera);
    context_delete(&gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
