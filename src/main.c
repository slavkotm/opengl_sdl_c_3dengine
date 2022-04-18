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

    struct shader *simple_shader = shader_malloc();
    struct shader *light_shader = shader_malloc();
    struct shader *model_shader = shader_malloc();

    shader_init(simple_shader, "shaders/basic.vert", "shaders/basic.frag", "r");
    shader_init(light_shader, "shaders/light.vert", "shaders/light.frag", "r");
    shader_init(model_shader, "shaders/model.vert", "shaders/model.frag", "r");

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(window, 640, 360);

    float move = 0.01f;
    glad_glEnable(GL_DEPTH_TEST);

    struct model *mdl = model_malloc();
    mdl->copy_meshes = mdl->meshes = NULL;
    mdl->textures_loaded = NULL;
    model_init(mdl, NULL, "assets/models/car.obj", true);

    
    struct texture *t = NULL, *tp = NULL;
/*    texture_push_back(&tp, 1, "1t", "1p");
    texture_push_back(&t, 2, "2t", "2p");
    tp->next = t;
    texture_push_back(&t, 3, "3t", "3p");
    texture_push_back(&t, 4, "4t", "4p");
    texture_push_back(&t, 5, "5t", "5p");
    texture_push_back(&t, 6, "6t", "6p");
    printf("%d\n", texture_list_size(tp));*/


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

/*    event_destroy(item_event);
    shader_destroy(simple_shader);
    camera_destroy(item_camera);
    free(item_camera);
    context_delete(&gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();*/

    return 0;
}
