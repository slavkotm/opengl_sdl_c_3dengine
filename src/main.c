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
#include "../header/mesh.h"
#include "../header/model.h"

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

struct DirectionalLight
{
    GLfloat direction[3];

    GLfloat ambient[3];
    GLfloat diffuse[3];
    GLfloat specular[3];
};

struct SpotLight
{
    GLfloat position[3];
    GLfloat direction[3];
    GLfloat cutoff;

    GLfloat ambient[3];
    GLfloat diffuse[3];
    GLfloat specular[3];

    float constant;
    float linear;
    float quadratic;
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
    struct shader *model_shader = shader_malloc();

    shader_init(simple_shader, "shaders/basic.vert", "shaders/basic.frag", "r");
    shader_init(light_shader, "shaders/light.vert", "shaders/light.frag", "r");
    shader_init(model_shader, "shaders/model.vert", "shaders/model.frag", "r");

/*    float cube[] = {

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
	stbi_uc* data = stbi_load("assets", &box_width, &box_height, &channels, 0);
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
	glad_glEnableVertexAttribArray(3);*/

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(window, 640, 360);

    float move = 0.01f;
    glad_glEnable(GL_DEPTH_TEST);

    /*GLfloat mm[16];
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

    struct Material materials[3];
    materials[0].ambient[0] = 0.25;
    materials[0].ambient[1] = 0.20725;
    materials[0].ambient[2] = 0.20725;

    materials[0].diffuse[0] = 1;
    materials[0].diffuse[1] = 0.829;
    materials[0].diffuse[2] = 0.829;

    materials[0].specular[0] = 0.296648;
    materials[0].specular[1] = 0.296648;
    materials[0].specular[2] = 0.296648;

    materials[0].shininess = 12.0f;

    materials[1].ambient[0] = 0.25;
    materials[1].ambient[1] = 0.25;
    materials[1].ambient[2] = 0.25;

    materials[1].diffuse[0] = 0.4;
    materials[1].diffuse[1] = 0.4;
    materials[1].diffuse[2] = 0.4;

    materials[1].specular[0] = 0.774597;
    materials[1].specular[1] = 0.774597;
    materials[1].specular[2] = 0.774597;

    materials[1].shininess = 77.0f;

    materials[2].ambient[0] = 0.1745;
    materials[2].ambient[1] = 0.01175;
    materials[2].ambient[2] = 0.01175;

    materials[2].diffuse[0] = 0.61424;
    materials[2].diffuse[1] = 0.04136;
    materials[2].diffuse[2] = 0.04136;

    materials[2].specular[0] = 0.727811;
    materials[2].specular[1] = 0.626959;
    materials[2].specular[2] = 0.626959;

    materials[2].shininess = 77.0f;



    struct PointLight light1;

    light1.position[0] = 0.0f;
    light1.position[1] = 0.0f;
    light1.position[2] = 0.0f;

    light1.ambient[0] = 0.2f;
    light1.ambient[1] = 0.2f;
    light1.ambient[2] = 0.2f;

    light1.diffuse[0] = 1.0f;
    light1.diffuse[1] = 1.0f;
    light1.diffuse[2] = 1.0f;

    light1.specular[0] = 3.0f;
    light1.specular[1] = 3.0f;
    light1.specular[2] = 3.0f;

    light1.constant = 0.9f;
    light1.linear = 0.1f;
    light1.quadratic = 0.09f;

    struct DirectionalLight light2;

    light2.direction[0] = -1.0f;
    light2.direction[1] = -1.0f;
    light2.direction[2] = -1.0f;

    light2.ambient[0] = 0.3f;
    light2.ambient[1] = 0.3f;
    light2.ambient[2] = 0.3f;

    light2.diffuse[0] = 0.6f;
    light2.diffuse[1] = 0.85f;
    light2.diffuse[2] = 1.0f;

    light2.specular[0] = 0.01f;
    light2.specular[1] = 0.01f;
    light2.specular[2] = 0.1f;


    struct SpotLight light3;

    light3.position[0] = -3.0f;
    light3.position[1] = -3.0f;
    light3.position[2] = -3.0f;

    light3.direction[0] = 1.0f;
    light3.direction[1] = 1.0f;
    light3.direction[2] = 1.0f;

    light3.cutoff = M_PI * 10.0f / 180.0f;


    float mmmoveee = 0.0f;
    float mmmrl = 0.0f;
    float mmmud = 0.0f;

    int count_cube = 200;
    float models[count_cube][16];
    float indexes[count_cube];
    for(int i = 0; i < count_cube; i++)
    {
        matrix_to_array(model, models[i], 4, 4);

        float scale = (rand() % 6 + 1) / (double)20;

        models[i][0] = scale;
        models[i][5] = scale;
        models[i][10] = scale; 

        models[i][12] = (rand() % 201 - 100) / 50.0;
        models[i][13] = (rand() % 201 - 100) / 50.0;
        models[i][14] = (rand() % 201 - 100) / 50.0; 
        indexes[i] = rand()%3;

        gsl_vector *vec = gsl_vector_alloc(3);
        gsl_vector_set(vec, 0, 0 - models[i][12]);
        gsl_vector_set(vec, 1, 0 - models[i][13]);
        gsl_vector_set(vec, 2, 0 - models[i][14]);

        if(vector_length(vec, 3) < 2.0)
            i--;
i    }*/


    
    struct model *mdl = model_malloc();
    model_init(mdl, NULL, "assets/De lorean DMC-12 model.obj", true);
    printf("%.4lf\n", mdl->copy_meshes->list_vertex->next->pos[0]);

    
/*    int size_index = index_list_size(mdl->copy_meshes->list_index) ;
   int size_vertex = vertex_list_size(mdl->copy_meshes->list_vertex) * 3 ;

    GLfloat arr_vertex[size_vertex];
    unsigned int arr_index[size_index];

    struct vertex *tmp_vertex = mdl->copy_meshes->list_vertex;
    struct index *tmp_index = mdl->copy_meshes->list_index;

    int i = 0;

    while(tmp_vertex != NULL)
    {
        arr_vertex[i] = tmp_vertex->pos[0];
        i++;
        arr_vertex[i] = tmp_vertex->pos[1];
        i++;
        arr_vertex[i] = tmp_vertex->pos[2];
        i++;
        printf("%d\n", i);
//        printf("agrargargeqgrggggggggggggggggg ::::::::::::::::::: %.4lf  %.4lf  %.4lf\n", tmp_vertex->pos[0], tmp_vertex->pos[1], tmp_vertex->pos[2]);
        arr_vertex[i] = tmp_vertex->norm[0];
        i++;
        arr_vertex[i] = tmp_vertex->norm[1];
        i++;
        arr_vertex[i] = tmp_vertex->norm[2];
        i++;

        arr_vertex[i] = tmp_vertex->texc[0];
        i++;
        arr_vertex[i] = tmp_vertex->texc[1];
        i++;

        arr_vertex[i] = tmp_vertex->tang[0];
        i++;
        arr_vertex[i] = tmp_vertex->tang[1];
        i++;
        arr_vertex[i] = tmp_vertex->tang[2];
        i++;
        
        arr_vertex[i] = tmp_vertex->bitang[0];
        i++;
        arr_vertex[i] = tmp_vertex->bitang[1];
        i++;
        arr_vertex[i] = tmp_vertex->bitang[2];
        i++;
        printf("AMRGMAOGRNAOWIGRNOQWNOGRINMRGIQMGN::::::::::::::::::;;;\n");
            tmp_vertex = tmp_vertex->next;
    }
printf("qwrgqwrHQGIUHGIUEHGRIUEQHRIUEIUGFRBIQEUGRNQUEIGRBQENUGRBQENIGRUHJQNIEUHRJGNIQUEHKRJG%d\n", size_vertex);
    unsigned int j = 0;
    while(j < size_index)
    {
        arr_index[j] = tmp_index->data;
        tmp_index = tmp_index->next;
        j++;
    }


    for(int i = 0;i<8;i++)
    {
        printf("%.4lf\n", arr_vertex[i]);
    }



//    printf("AERGHOEQHGRJOQEJNGRIUEQJNRIGUQEIRGIEQUGRIQENGRQENGKJFNGKJEWNGFRIJKEQNKFGJNKRJEGNQEIGR ::::::::::::: %d\n", mdl->copy_meshes->VAO);
    //printf("AERGHOEQHGRJOQEJNGRIUEQJNRIGUQEIRGIEQUGRIQENGRQENGKJFNGKJEWNGFRIJKEQNKFGJNKRJEGNQEIGR ::::::::::::: %d\n", mdl->copy_meshes->next->VAO);
//    printf("AERGHOEQHGRJOQEJNGRIUEQJNRIGUQEIRGIEQUGRIQENGRQENGKJFNGKJEWNGFRIJKEQNKFGJNKRJEGNQEIGR ::::::::::::: %d\n", mdl->copy_meshes->next->next->VAO);
    //printf("AERGHOEQHGRJOQEJNGRIUEQJNRIGUQEIRGIEQUGRIQENGRQENGKJFNGKJEWNGFRIJKEQNKFGJNKRJEGNQEIGR ::::::::::::: %d\n", mesh_list_size(mdl->copy_meshes));
    //printf("%d\n", mesh_list_size(mdl->meshes));
    //for(int i = 0; i < mesh_list_size(mdl->meshes); i++)


    GLfloat mdls[16] ;

    



    GLfloat polygon[8 * (3 + 3)] = {
        -1.0f,  1.0f, -1.0f,     1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, -1.0f,     0.0f, 1.0f, 0.0f,   
         1.0f,  1.0f,  1.0f,     0.0f, 0.0f, 1.0f,    
        -1.0f,  1.0f,  1.0f,     0.0f, 1.0f, 1.0f,  
        -1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 1.0f,  
         1.0f, -1.0f, -1.0f,     1.0f, 0.2f, 1.0f, 
         1.0f, -1.0f,  1.0f,     0.4f, 0.0f, 1.0f,  
        -1.0f, -1.0f,  1.0f,     1.0f, 1.0f, 1.0f  
    };

    GLuint indeces[] = {
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

unsigned int VBO_polygon, EBO_polygon;
    glGenBuffers(1, &VBO_polygon);
    glGenBuffers(1, &EBO_polygon);

    GLuint VAO_polygon;
    glGenVertexArrays(1, &VAO_polygon);

    glBindVertexArray(VAO_polygon);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * size_index, arr_index, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size_vertex, arr_vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);


    int size_index1 = index_list_size(mdl->copy_meshes->next->list_index) ;
   int size_vertex1 = vertex_list_size(mdl->copy_meshes->next->list_vertex) * 3;

    GLfloat arr_vertex1[size_vertex1];
    unsigned int arr_index1[size_index1];

    struct vertex *tmp_vertex1 = mdl->copy_meshes->next->list_vertex;
    struct index *tmp_index1 = mdl->copy_meshes->next->list_index;

    int i1 = 0;

    while(i1 < size_vertex1)
    {
        arr_vertex1[i1] = tmp_vertex1->pos[0];
        i1++;
        arr_vertex1[i1] = tmp_vertex1->pos[1];
        i1++;
        arr_vertex1[i1] = tmp_vertex1->pos[2];
        i1++;
//        printf("agrargargeqgrggggggggggggggggg ::::::::::::::::::: %.4lf  %.4lf  %.4lf\n", tmp_vertex->pos[0], tmp_vertex->pos[1], tmp_vertex->pos[2]);
        arr_vertex[i] = tmp_vertex->norm[0];
        i++;
        arr_vertex[i] = tmp_vertex->norm[1];
        i++;
        arr_vertex[i] = tmp_vertex->norm[2];
        i++;

        arr_vertex[i] = tmp_vertex->texc[0];
        i++;
        arr_vertex[i] = tmp_vertex->texc[1];
        i++;

        arr_vertex[i] = tmp_vertex->tang[0];
        i++;
        arr_vertex[i] = tmp_vertex->tang[1];
        i++;
        arr_vertex[i] = tmp_vertex->tang[2];
        i++;
        
        arr_vertex[i] = tmp_vertex->bitang[0];
        i++;
        arr_vertex[i] = tmp_vertex->bitang[1];
        i++;
        arr_vertex[i] = tmp_vertex->bitang[2];
        i++;
            tmp_vertex1 = tmp_vertex1->next;
    }
printf("qwrgqwrHQGIUHGIUEHGRIUEQHRIUEIUGFRBIQEUGRNQUEIGRBQENUGRBQENIGRUHJQNIEUHRJGNIQUEHKRJG%d\n", size_vertex1);
    unsigned int j1 = 0;
    while(j1 < size_index1)
    {
        arr_index1[j1] = tmp_index1->data;
        tmp_index1 = tmp_index1->next;
        j1++;
    }


    for(int i = 0;i<8;i++)
    {
        printf("%.4lf\n", arr_vertex1[i]);
    }



//    printf("AERGHOEQHGRJOQEJNGRIUEQJNRIGUQEIRGIEQUGRIQENGRQENGKJFNGKJEWNGFRIJKEQNKFGJNKRJEGNQEIGR ::::::::::::: %d\n", mdl->copy_meshes->VAO);
    //printf("AERGHOEQHGRJOQEJNGRIUEQJNRIGUQEIRGIEQUGRIQENGRQENGKJFNGKJEWNGFRIJKEQNKFGJNKRJEGNQEIGR ::::::::::::: %d\n", mdl->copy_meshes->next->VAO);
//    printf("AERGHOEQHGRJOQEJNGRIUEQJNRIGUQEIRGIEQUGRIQENGRQENGKJFNGKJEWNGFRIJKEQNKFGJNKRJEGNQEIGR ::::::::::::: %d\n", mdl->copy_meshes->next->next->VAO);
    //printf("AERGHOEQHGRJOQEJNGRIUEQJNRIGUQEIRGIEQUGRIQENGRQENGKJFNGKJEWNGFRIJKEQNKFGJNKRJEGNQEIGR ::::::::::::: %d\n", mesh_list_size(mdl->copy_meshes));
    //printf("%d\n", mesh_list_size(mdl->meshes));
    //for(int i = 0; i < mesh_list_size(mdl->meshes); i++)



unsigned int VBO_polygon1, EBO_polygon1;
    glGenBuffers(1, &VBO_polygon1);
    glGenBuffers(1, &EBO_polygon1);

    GLuint VAO_polygon1;
    glGenVertexArrays(1, &VAO_polygon1);

    glBindVertexArray(VAO_polygon1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_polygon1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_polygon1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * size_index1, arr_index1, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size_vertex1, arr_vertex1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);*/



    GLfloat mdls[16] ;

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
/*
        //mm[12] = move;

        mm[0] = cos(M_PI * move * 50 / 180.0);
        mm[8] = -sin(M_PI * move * 50 / 180.0);
        mm[2] = sin(M_PI * move * 50/ 180.0);
        mm[10] = cos(M_PI * move * 50/ 180.0);

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

        light1.specular[0] = (sin(move * (double)2) + (double)1);
        light1.specular[1] = (sin(move * (double)2 + (double)2 * M_PI / (double)3) + (double)1);
        light1.specular[2] = (sin(move * (double)2 + (double)4 * M_PI / (double)3) + (double)1) ;

        light1.diffuse[0] = 0.8 * light1.specular[0];
        light1.diffuse[1] = 0.8 * light1.specular[1];
        light1.diffuse[2] = 0.8 * light1.specular[2];

        light1.ambient[0] = 0.4 * light1.specular[0];
        light1.ambient[1] = 0.4 * light1.specular[1];
        light1.ambient[2] = 0.4 * light1.specular[2];
        
        move += 0.01;
       
        for(int i = 0; i < count_cube; i++)
        {
        gsl_cgp = camera_get_position(item_camera);

        cgp[0] = gsl_vector_get(gsl_cgp, 0);
        cgp[1] = gsl_vector_get(gsl_cgp, 1);
        cgp[2] = gsl_vector_get(gsl_cgp, 2);

        shader_use(simple_shader);

        //matrix_to_array(model, models[i], 4, 4);
        

        shader_set_matrix4f(simple_shader, "model", models[i]);
        shader_set_matrix4f(simple_shader, "view", a);
        shader_set_matrix4f(simple_shader, "projection", p);
        shader_set_bool(simple_shader, "wireframeMode", event_get_space(item_event));
        shader_set_vec3(simple_shader, "light.position", light2.direction[0], light2.direction[1], light2.direction[2]);

        shader_set_vec3(simple_shader, "light.ambient", light2.ambient[0], light2.ambient[1], light2.ambient[2]);
        shader_set_vec3(simple_shader, "light.diffuse", light2.diffuse[0], light2.diffuse[1], light2.diffuse[2]);
        shader_set_vec3(simple_shader, "light.specular", light2.specular[0], light2.specular[1], light2.specular[2]);

        shader_set_vec3(simple_shader, "material.ambient", materials[(int)indexes[i]].ambient[0], materials[(int)indexes[i]].ambient[1], materials[(int)indexes[i]].ambient[2]);
        shader_set_vec3(simple_shader, "material.diffuse", materials[(int)indexes[i]].diffuse[0], materials[(int)indexes[i]].diffuse[1], materials[(int)indexes[i]].diffuse[2]);
        shader_set_vec3(simple_shader, "material.specular", materials[(int)indexes[i]].specular[0], materials[(int)indexes[i]].specular[1], materials[(int)indexes[i]].specular[2]);
        shader_set_float(simple_shader, "material.shininess", materials[(int)indexes[i]].shininess);

        shader_set_vec3(simple_shader, "viewPos", cgp[0], cgp[1], cgp[2]);

        //shader_set_float(simple_shader, "light.constant", light1.constant);
        //shader_set_float(simple_shader, "light.linear", light1.linear);
        //shader_set_float(simple_shader, "light.quadratic", light1.quadratic);
        shader_set_int(simple_shader, "light.type", 1);

        //glBindVertexArray(VAO_polygon);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/

        matrix_to_array(model, mdls, 4, 4);
  //      mdls[0] = 0.1;
        //mdls[5] = ;
 //       mdls[10] = 0.08;
        shader_use(model_shader);
        shader_set_matrix4f(model_shader, "model", mdls);
        shader_set_matrix4f(model_shader, "view", a);
        shader_set_matrix4f(model_shader, "projection", p);
        shader_set_bool(model_shader, "wireframeMode", event_get_space(item_event));
        model_draw(mdl, model_shader);
        //glBindVertexArray(VAO_polygon);
        //glDrawElements(GL_TRIANGLES, size_index, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);
        //glBindVertexArray(VAO_polygon1);
        //glDrawElements(GL_TRIANGLES, size_index1, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        /*gsl_cgp = camera_get_position(item_camera);

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
        glDrawArrays(GL_TRIANGLES, 0, 36);*/


/*        matrix_to_array(model1, mm1, 4, 4);
        mm1[12] = light1.position[0];
        mm1[13] = light1.position[1];
        mm1[14] = light1.position[2];

        mm1[0] = 0.1f;
        mm1[5] = 0.1f;
        mm1[10] = 0.1f;
        //mm[12] = move;

        shader_use(light_shader);
        shader_set_matrix4f(light_shader, "model", mm1);
        shader_set_matrix4f(light_shader, "view", a);
        shader_set_matrix4f(light_shader, "projection", p);
        shader_set_vec3(light_shader, "lightColor", light1.specular[0], light1.specular[1], light1.specular[2]);*/

//        glBindVertexArray(VAO_polygon);
//        glDrawArrays(GL_TRIANGLES, 0, 36);

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
