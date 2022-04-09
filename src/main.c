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

struct Material
{
    GLfloat ambient[3];
    GLfloat diffuse[3];
    GLfloat specular[3];
    GLfloat shininess;
};

struct PointLight
{
    GLfloat position[3];

    GLfloat ambient[3];
    GLfloat diffuse[3];
    GLfloat specular[3];

    float constant;
    float linear;
    float quadratic;
};

struct DirecationalLight
{
    GLfloat direction[3];

    GLfloat ambient[3];
    GLfloat diffuse[3];
    GLfloat specular[3];
};

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
    GLfloat mmc1[16];
    GLfloat mmc2[16];
    GLfloat mm1[16];
    GLfloat cgp[3];
    gsl_vector *gsl_cgp = gsl_vector_alloc(3);

    struct Material cubeMat1;
    cubeMat1.ambient[0] = 0.25;
    cubeMat1.ambient[1] = 0.20725;
    cubeMat1.ambient[2] = 0.20725;

    cubeMat1.diffuse[0] = 1;
    cubeMat1.diffuse[1] = 0.829;
    cubeMat1.diffuse[2] = 0.829;

    cubeMat1.specular[0] = 0.296648;
    cubeMat1.specular[1] = 0.296648;
    cubeMat1.specular[2] = 0.296648;

    cubeMat1.shininess = 12.0f;
    // perl

    struct Material cubeMat2;
    cubeMat2.ambient[0] = 0.25;
    cubeMat2.ambient[1] = 0.25;
    cubeMat2.ambient[2] = 0.25;

    cubeMat2.diffuse[0] = 0.4;
    cubeMat2.diffuse[1] = 0.4;
    cubeMat2.diffuse[2] = 0.4;

    cubeMat2.specular[0] = 0.774597;
    cubeMat2.specular[1] = 0.774597;
    cubeMat2.specular[2] = 0.774597;

    cubeMat2.shininess = 77.0f;
    // chrome

    struct Material cubeMat3;
    cubeMat3.ambient[0] = 0.1745;
    cubeMat3.ambient[1] = 0.01175;
    cubeMat3.ambient[2] = 0.01175;

    cubeMat3.diffuse[0] = 0.61424;
    cubeMat3.diffuse[1] = 0.04136;
    cubeMat3.diffuse[2] = 0.04136;

    cubeMat3.specular[0] = 0.727811;
    cubeMat3.specular[1] = 0.626959;
    cubeMat3.specular[2] = 0.626959;

    cubeMat3.shininess = 77.0f;


    struct PointLight light1;

    light1.position[0] = 0.0f;
    light1.position[1] = 0.0f;
    light1.position[2] = 0.0f;

    light1.ambient[0] = 0.2f;
    light1.ambient[1] = 0.2f;
    light1.ambient[2] = 0.2f;

    light1.diffuse[0] = 0.5f;
    light1.diffuse[1] = 0.5f;
    light1.diffuse[2] = 0.5f;

    light1.specular[0] = 1.0f;
    light1.specular[1] = 1.0f;
    light1.specular[2] = 1.0f;

    light1.constant = 1.0f;
    light1.linear = 0.14f;
    light1.quadratic = 0.07f;

    float mmmoveee = 5.0f;
    float mmmrl = 5.0f;
    float mmmud = 0.0f;

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

        //mm[12] = move;

        /*mm[0] = cos(M_PI * move * 50 / 180.0);
        mm[8] = -sin(M_PI * move * 50 / 180.0);
        mm[2] = sin(M_PI * move * 50/ 180.0);
        mm[10] = cos(M_PI * move * 50/ 180.0);*/

        const Uint8 *key_state = SDL_GetKeyboardState(NULL); 
        if(key_state[SDL_SCANCODE_UP])
            mmmoveee -= 0.05;
        if(key_state[SDL_SCANCODE_DOWN])
            mmmoveee += 0.05;
        if(key_state[SDL_SCANCODE_LEFT])
            mmmrl -= 0.05;
        if(key_state[SDL_SCANCODE_RIGHT])
            mmmrl += 0.05;
        if(key_state[SDL_SCANCODE_PAGEUP])
            mmmud += 0.05;
        if(key_state[SDL_SCANCODE_PAGEDOWN])
            mmmud -= 0.05;

        light1.position[0] = mmmrl;// * cos(move * 1.2f);
        light1.position[1] = mmmud;
        light1.position[2] = mmmoveee;// * sin(move * 1.2f);

        /*light1.specular[0] = (sin(move * (double)2) + (double)1);
        light1.specular[1] = (sin(move * (double)2 + (double)2 * M_PI / (double)3) + (double)1);
        light1.specular[2] = (sin(move * (double)2 + (double)4 * M_PI / (double)3) + (double)1) ;

        light1.diffuse[0] = 0.8 * light1.specular[0];
        light1.diffuse[1] = 0.8 * light1.specular[1];
        light1.diffuse[2] = 0.8 * light1.specular[2];

        light1.ambient[0] = 0.4 * light1.specular[0];
        light1.ambient[1] = 0.4 * light1.specular[1];
        light1.ambient[2] = 0.4 * light1.specular[2];*/

        move += 0.01;
       

        gsl_cgp = camera_get_position(item_camera);

        cgp[0] = gsl_vector_get(gsl_cgp, 0);
        cgp[1] = gsl_vector_get(gsl_cgp, 1);
        cgp[2] = gsl_vector_get(gsl_cgp, 2);

        shader_use(simple_shader);
        matrix_to_array(model, mm, 4, 4);
        shader_set_matrix4f(simple_shader, "model", mm);
        shader_set_matrix4f(simple_shader, "view", a);
        shader_set_matrix4f(simple_shader, "projection", p);
        shader_set_bool(simple_shader, "wireframeMode", event_get_space(item_event));
        shader_set_vec3(simple_shader, "light.position", light1.position[0], light1.position[1], light1.position[2]);

        shader_set_vec3(simple_shader, "light.ambient", light1.ambient[0], light1.ambient[1], light1.ambient[2]);
        shader_set_vec3(simple_shader, "light.diffuse", light1.diffuse[0], light1.diffuse[1], light1.diffuse[2]);
        shader_set_vec3(simple_shader, "light.specular", light1.specular[0], light1.specular[1], light1.specular[2]);

        shader_set_vec3(simple_shader, "material.ambient", cubeMat1.ambient[0], cubeMat1.ambient[1], cubeMat1.ambient[2]);
        shader_set_vec3(simple_shader, "material.diffuse", cubeMat1.diffuse[0], cubeMat1.diffuse[1], cubeMat1.diffuse[2]);
        shader_set_vec3(simple_shader, "material.specular", cubeMat1.specular[0], cubeMat1.specular[1], cubeMat1.specular[2]);
        shader_set_float(simple_shader, "material.shininess", cubeMat1.shininess);

        shader_set_vec3(simple_shader, "viewPos", cgp[0], cgp[1], cgp[2]);

        shader_set_float(simple_shader, "light.constant", light1.constant);
        shader_set_float(simple_shader, "light.linear", light1.linear);
        shader_set_float(simple_shader, "light.quadratic", light1.quadratic);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        shader_use(simple_shader);
        matrix_to_array(model, mmc1, 4, 4);
        mmc1[12] = 3.0f;
        shader_set_matrix4f(simple_shader, "model", mmc1);
        shader_set_matrix4f(simple_shader, "view", a);
        shader_set_matrix4f(simple_shader, "projection", p);
        shader_set_bool(simple_shader, "wireframeMode", event_get_space(item_event));
        shader_set_vec3(simple_shader, "light.position", light1.position[0], light1.position[1], light1.position[2]);
        shader_set_vec3(simple_shader, "light.ambient", light1.ambient[0], light1.ambient[1], light1.ambient[2]);
        shader_set_vec3(simple_shader, "light.diffuse", light1.diffuse[0], light1.diffuse[1], light1.diffuse[2]);
        shader_set_vec3(simple_shader, "light.specular", light1.specular[0], light1.specular[1], light1.specular[2]);
        shader_set_vec3(simple_shader, "material.ambient", cubeMat2.ambient[0], cubeMat2.ambient[1], cubeMat2.ambient[2]);
        shader_set_vec3(simple_shader, "material.diffuse", cubeMat2.diffuse[0], cubeMat2.diffuse[1], cubeMat2.diffuse[2]);
        shader_set_vec3(simple_shader, "material.specular", cubeMat2.specular[0], cubeMat2.specular[1], cubeMat2.specular[2]);
        shader_set_float(simple_shader, "material.shininess", cubeMat2.shininess);
        shader_set_vec3(simple_shader, "viewPos", cgp[0], cgp[1], cgp[2]);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader_use(simple_shader);
        matrix_to_array(model, mmc2, 4, 4);
        mmc2[12] = -3.0f;
        shader_set_matrix4f(simple_shader, "model", mmc2);
        shader_set_matrix4f(simple_shader, "view", a);
        shader_set_matrix4f(simple_shader, "projection", p);
        shader_set_bool(simple_shader, "wireframeMode", event_get_space(item_event));
        shader_set_vec3(simple_shader, "light.position", light1.position[0], light1.position[1], light1.position[2]);
        shader_set_vec3(simple_shader, "light.ambient", light1.ambient[0], light1.ambient[1], light1.ambient[2]);
        shader_set_vec3(simple_shader, "light.diffuse", light1.diffuse[0], light1.diffuse[1], light1.diffuse[2]);
        shader_set_vec3(simple_shader, "light.specular", light1.specular[0], light1.specular[1], light1.specular[2]);
        shader_set_vec3(simple_shader, "material.ambient", cubeMat3.ambient[0], cubeMat3.ambient[1], cubeMat3.ambient[2]);
        shader_set_vec3(simple_shader, "material.diffuse", cubeMat3.diffuse[0], cubeMat3.diffuse[1], cubeMat3.diffuse[2]);
        shader_set_vec3(simple_shader, "material.specular", cubeMat3.specular[0], cubeMat3.specular[1], cubeMat3.specular[2]);
        shader_set_float(simple_shader, "material.shininess", cubeMat3.shininess);
        shader_set_vec3(simple_shader, "viewPos", cgp[0], cgp[1], cgp[2]);

        glBindVertexArray(VAO_polygon);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        matrix_to_array(model1, mm1, 4, 4);
        mm1[12] = light1.position[0];
        mm1[13] = light1.position[1];
        mm1[14] = light1.position[2];

        mm1[0] = 0.2f;
        mm1[5] = 0.2f;
        mm1[10] = 0.2f;
        //mm[12] = move;

        shader_use(light_shader);
        shader_set_matrix4f(light_shader, "model", mm1);
        shader_set_matrix4f(light_shader, "view", a);
        shader_set_matrix4f(light_shader, "projection", p);
        shader_set_vec3(light_shader, "lightColor", light1.specular[0], light1.specular[1], light1.specular[2]);

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
