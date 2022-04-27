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
#include "../header/skybox.h"
#include "../header/bloom.h"

int main(int argc, char **argv)
{
    init(RENDER_INIT, AUDIO_INIT);
    SDL_Window *window = window_create(WINDOW_NAME, WINDOW_X_POSITION_SCREEN, WINDOW_Y_POSITION_SCREEN, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_RENDERER_ACCELERATED);
    SDL_GLContext gl_context = context_create(window);
    context_set(MAJOR_VERSION, MINOR_VERSION);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    gsl_vector *vector_position = gsl_vector_alloc(3);
    gsl_vector *vector_front = gsl_vector_alloc(3);
    gsl_vector *vector_up = gsl_vector_alloc(3);
    gsl_vector *vector_world_up = gsl_vector_alloc(3);

    gsl_vector_set(vector_position, 0, (double)0.258735);
    gsl_vector_set(vector_position, 1, (double)0.615317);
    gsl_vector_set(vector_position, 2, (double)-11.536388);

    gsl_vector_set(vector_front, 0, (double)-0.043549);
    gsl_vector_set(vector_front, 1, (double)-0.056693);
    gsl_vector_set(vector_front, 2, (double)0.997441);

    gsl_vector_set(vector_up, 0, (double)0);
    gsl_vector_set(vector_up, 1, (double)1);
    gsl_vector_set(vector_up, 2, (double)0);

    vector_world_up = vector_up;

    struct camera *item_camera = camera_malloc();
    camera_constructor_assignment(item_camera, vector_position, vector_front, vector_up, vector_world_up, YAW, PITCH, FOV, ZNEAR, ZFAR, ASPECTRATIO,SPEED, SENSITIVITY); 

    gsl_matrix *model = camera_get_model_matrix();
    struct event *item_event = event_malloc();
    event_init(item_event, item_camera, true, 0, 0, 0, 0, 0, 0, 0, false, false);

    struct shader *skybox_shader = shader_malloc();
    shader_init(skybox_shader, "shaders/skybox.vert", "shaders/skybox.frag", "r");

    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(window, SCREEN_WIDTH, SCREEN_HEIGHT);

    struct cubemap *skybox_item = cubemap_malloc();
    cubemap_mesh(&skybox_item);
    cubemap_init(skybox_item);
    skybox_item->cubemap_texture = cubemap_load(&skybox_item, 6);
    shader_use(skybox_shader);
    shader_set_int(skybox_shader, "skybox", 0);

    GLuint hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	
    unsigned int colorBuffers[2];
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
    }
	
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);
	
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer not complete!\n");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("Framebuffer not complete!\n");;
    }
    struct model *sp = model_malloc();
    struct model *pl = model_malloc();
    struct model *slk = model_malloc();
    struct model *koleso_front = model_malloc();
    struct model *koleso_back = model_malloc();
    struct model *axes_front = model_malloc();
    struct model *axes_back = model_malloc();
    struct model *lamp_front = model_malloc();
    struct model *lamp_back_orange = model_malloc();
    struct model *lamp_back_yellow = model_malloc();
    struct model *plane = model_malloc();
    struct model *wireframe = model_malloc();

    model_init(sp, NULL, "assets/models/sphere/sphere.obj", true);
    model_init(pl, NULL, "assets/models/plane/plane.obj", true);
    model_init(slk, NULL, "assets/models/car/frame/frame.obj", true);
    model_init(koleso_front, NULL, "assets/models/car/wheels/wheel_front/wheel_front.obj", true);
    model_init(koleso_back, NULL, "assets/models/car/wheels/wheel_back/wheel_back.obj", true);
    model_init(axes_front, NULL, "assets/models/car/axes/axis_front/axis_front.obj", true);
    model_init(axes_back, NULL, "assets/models/car/axes/axis_back/axis_back.obj", true);
    model_init(lamp_front, NULL, "assets/models/car/lamps/lamp_front/lamp_front.obj", true);
    model_init(lamp_back_orange, NULL, "assets/models/car/lamps/lamp_back/lamps_orange/lamp_orange.obj", true);
    model_init(lamp_back_yellow, NULL, "assets/models/car/lamps/lamp_back/lamps_yellow/lamp_yellow.obj", true);
    model_init(plane, NULL, "assets/models/plane/plane.obj", true);
    model_init(wireframe, NULL, "assets/models/car/wireframe/wireframe.obj", true);

    GLfloat aslk[16];

    struct shader *shader_bloom = shader_malloc();
    struct shader *shader_blur = shader_malloc();
    struct shader *shader_bloom_final = shader_malloc();
    struct shader *shader_light = shader_malloc();
    shader_init(shader_bloom, "shaders/bloom.vs", "shaders/bloom.fs", "r");
    shader_init(shader_blur, "shaders/blur.vs", "shaders/blur.fs", "r");
    shader_init(shader_bloom_final, "shaders/bloom_final.vs", "shaders/bloom_final.fs", "r");
    shader_init(shader_light, "shaders/bloom.vs", "shaders/light_box.fs", "r");
    
    struct bloom *bloom_item = bloom_malloc();
    bloom_init(&bloom_item);
    bloom_hdr_buffer(&bloom_item);
    bloom_color_buffers(&bloom_item);
    bloom_rbo_depth(&bloom_item);
    bloom_attachments(&bloom_item);
    bloom_ping_pong_buffers(&bloom_item);
    bloom_light_positions(&bloom_item);
    bloom_light_colors(&bloom_item);

    shader_use(shader_bloom);
    shader_set_int(shader_bloom, "diffuseTexture", 0);
    shader_use(shader_blur);
    shader_set_int(shader_blur, "image", 0);
    shader_use(shader_bloom_final);
    shader_set_int(shader_bloom_final, "scene", 0);
    shader_set_int(shader_bloom_final, "bloomBlur", 1);
    
    glEnable(GL_DEPTH_TEST);

    float light_colors[6][3];
    light_colors[0][0] = 10.0f;
    light_colors[0][1] = 10.0f;
    light_colors[0][2] = 0.0f;

    light_colors[1][0] = 2.0f;
    light_colors[1][1] = 0.8f;
    light_colors[1][2] = 1.898f;

    light_colors[2][0] = 1.2f;
    light_colors[2][1] = 1.0f;
    light_colors[2][2] = 1.4f;

    light_colors[3][0] = 0.0f;
    light_colors[3][1] = 2.0f;
    light_colors[3][2] = 2.0f;

    light_colors[4][0] = 10.0f;
    light_colors[4][1] = 0.7f;
    light_colors[4][2] = 0.4f;

    light_colors[5][0] = 5.0f;
    light_colors[5][1] = 0.0f;
    light_colors[5][2] = 0.0f;

    struct shader *lighting_shader = shader_malloc();
    shader_init(lighting_shader, "shaders/material.vert", "shaders/material.frag", "r");

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    float pw[10][16];
        
    item_camera->position->data[0] = 0.515069f;
    item_camera->position->data[1] = 0.386305f;
    item_camera->position->data[2] = 2.650963f;

    item_camera->front->data[0] = -0.177942f;
    item_camera->front->data[1] = 0.004363f;
    item_camera->front->data[2] = 0.984031f;

    float dvig = 0.0f;
    float dv = 0.0f;
    float aslk1[16];
    gsl_matrix *matrix_rotate = gsl_matrix_alloc(4, 4);
    gsl_matrix *matrix_res = gsl_matrix_alloc(4, 4);
    
    while(event_get_running(item_event))
    {
        event_handle(item_event);
        camera_move(item_camera, event_get_dir(item_event));
        camera_rotate(item_camera, event_get_x_off_set(item_event), -event_get_y_off_set(item_event));
        camera_set_cull_face_mode(event_get_cull_face(item_event));
        camera_set_polygon_mode(event_get_space(item_event));
        
        GLfloat view[16]; 
        matrix_to_array(camera_get_view_matrix(item_camera), view, 4, 4);

        GLfloat vvvv[16]; 
        matrix_to_array(camera_get_view_matrix(item_camera), vvvv, 4, 4);
        
        GLfloat projection[16];
        matrix_to_array(camera_get_projection_matrix(item_camera), projection, 4, 4);

        if(!event_get_flag(item_event))
        {
            item_camera->position->data[0] = 0.515069f;
            item_camera->position->data[1] = 0.386305f;
            item_camera->position->data[2] = 2.650963f + dvig;
            item_camera->front->data[0] = -0.177942f;
            item_camera->front->data[1] = 0.004363f;
            item_camera->front->data[2] = 0.984031f;
        }

        glad_glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_use(shader_bloom);
        shader_set_matrix4f(shader_bloom, "projection", projection);
        shader_set_matrix4f(shader_bloom, "view", view);
        glActiveTexture(GL_TEXTURE0);

        shader_use(lighting_shader);
        shader_set_vec3(lighting_shader, "light.position", 1.0f, 1.0f, 1.0f);
        shader_set_vec3(lighting_shader, "viewPos", item_camera->position->data[0], item_camera->position->data[1], item_camera->position->data[2]);

        printf("%.6lf %.6lf %.6lf\n", item_camera->front->data[0], item_camera->front->data[1], item_camera->front->data[2]);

        shader_set_vec3(lighting_shader, "light.ambient", 0.12f, 0.01f, 0.2f);
        shader_set_vec3(lighting_shader, "light.diffuse", 0.6f, 0.5f, 1.0f);
        shader_set_vec3(lighting_shader, "light.specular", 2.0f, 0.8f, 1.898f);

        shader_set_vec3(lighting_shader, "material.ambient", 0.25f, 0.25f, 0.25f);
        shader_set_vec3(lighting_shader, "material.diffuse", 0.4f, 0.4f, 0.4f);
        shader_set_vec3(lighting_shader, "material.specular", 0.774597f, 0.774597f, 0.774597f);
        shader_set_float(lighting_shader, "material.shininess", 76.8f);

        gsl_matrix_set_zero(matrix_rotate);
        gsl_matrix_set(matrix_rotate, 0, 0, 1);
        gsl_matrix_set(matrix_rotate, 1, 1, cos(dvig * M_PI * 360.0 / 180.0));
        gsl_matrix_set(matrix_rotate, 1, 2, sin(dvig * M_PI * 360.0/ 180.0));
        gsl_matrix_set(matrix_rotate, 2, 1,-sin(dvig * M_PI * 360.0/ 180.0));
        gsl_matrix_set(matrix_rotate, 2, 2, cos(dvig * M_PI * 360.0/ 180.0));
        gsl_matrix_set(matrix_rotate, 3, 3, 1);

        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1, model, matrix_rotate, 0, matrix_res);

        gsl_matrix_set(matrix_res, 0, 0, 0.05f * gsl_matrix_get(matrix_res, 0, 0));
        gsl_matrix_set(matrix_res, 1, 1, 0.05f * gsl_matrix_get(matrix_res, 1, 1));
        gsl_matrix_set(matrix_res, 1, 2, 0.05f * gsl_matrix_get(matrix_res, 1, 2));
        gsl_matrix_set(matrix_res, 2, 1, 0.05f * gsl_matrix_get(matrix_res, 2, 1));
        gsl_matrix_set(matrix_res, 2, 2, 0.05f * gsl_matrix_get(matrix_res, 2, 2));

        matrix_to_array(model, aslk, 4, 4);
        aslk[0] = 0.05f;
        aslk[5] = 0.05f;
        aslk[10] = 0.05f;
        aslk[12] = -0.25f;
        aslk[13] = 0.23f;
        aslk[14] = 5.0f + dvig;
        shader_use(lighting_shader);
        shader_set_matrix4f(lighting_shader, "model", aslk);
        shader_set_matrix4f(lighting_shader, "view", view);
        shader_set_matrix4f(lighting_shader, "projection", projection);
        shader_set_bool(lighting_shader, "wireframemode", event_get_space(item_event));
        model_draw(slk, lighting_shader);

        matrix_to_array(matrix_res, aslk1, 4, 4);
        aslk1[12] = -0.185f;
        aslk1[13] = 0.115f;
        aslk1[14] = 5.515f + dvig;
        dv += 0.2f;
        shader_use(lighting_shader);
        shader_set_matrix4f(lighting_shader, "model", aslk1);
        shader_set_matrix4f(lighting_shader, "view", view);
        shader_set_matrix4f(lighting_shader, "projection", projection);
        shader_set_bool(lighting_shader, "wireframemode", event_get_space(item_event));
        model_draw(koleso_front, lighting_shader);

        shader_use(shader_light);
        shader_set_matrix4f(shader_light, "model", aslk1);
        shader_set_matrix4f(shader_light, "view", view);
        shader_set_matrix4f(shader_light, "projection", projection);
        shader_set_vec3_(shader_light, "lightColor", light_colors[3]);
        model_draw(axes_front, shader_light);

        aslk1[12] = -0.185f;
        aslk1[13] = 0.135f;
        aslk1[14] = 4.515f + dvig;
        shader_use(lighting_shader);
        shader_set_matrix4f(lighting_shader, "model", aslk1);
        shader_set_matrix4f(lighting_shader, "view", view);
        shader_set_matrix4f(lighting_shader, "projection", projection);
        shader_set_bool(lighting_shader, "wireframemode", event_get_space(item_event));
        model_draw(koleso_back, lighting_shader);

        shader_use(shader_light);
        shader_set_matrix4f(shader_light, "model", aslk1);
        shader_set_matrix4f(shader_light, "view", view);
        shader_set_matrix4f(shader_light, "projection", projection);
        shader_set_vec3_(shader_light, "lightColor", light_colors[3]);
        model_draw(axes_back, shader_light);
        float move = 8.0f;
        dvig += 0.03f;

        for(int i = 0; i < 10; i++)
        {
            matrix_to_array(model, pw[i], 4, 4);
            pw[i][0] = 0.5f;
            pw[i][5] = 0.5f;
            pw[i][10] = 0.5f;
            pw[i][14] = move;
            move += 8.0f;
            shader_use(lighting_shader);
            shader_set_matrix4f(lighting_shader, "model", pw[i]);
            shader_set_matrix4f(lighting_shader, "view", view);
            shader_set_matrix4f(lighting_shader, "projection", projection);
            model_draw(plane, lighting_shader);

            shader_use(shader_light);
            shader_set_matrix4f(shader_light, "model", pw[i]);
            shader_set_matrix4f(shader_light, "view", view);
            shader_set_matrix4f(shader_light, "projection", projection);
            shader_set_vec3_(shader_light, "lightColor", light_colors[1]);
            model_draw(wireframe, shader_light);
        }

        shader_use(shader_light);
        pw[9][14] += 10.0f;
        pw[9][0] = 2.0f;
        pw[9][5] = 2.0f;
        pw[9][10] = 2.0f;
        shader_set_matrix4f(shader_light, "model", pw[9]);
        shader_set_matrix4f(shader_light, "view", view);
        shader_set_matrix4f(shader_light, "projection", projection);
        shader_set_vec3_(shader_light, "lightColor", light_colors[1]);
        model_draw(sp, shader_light);

        aslk[0] = 0.05f;
        aslk[5] = 0.05f;
        aslk[10] = 0.05f;


        shader_set_matrix4f(shader_light, "model", aslk);
        shader_set_matrix4f(shader_light, "view", view);
        shader_set_matrix4f(shader_light, "projection", projection);
        shader_set_vec3_(shader_light, "lightColor", light_colors[4]);
        model_draw(lamp_front, shader_light);

        shader_set_matrix4f(shader_light, "model", aslk);
        shader_set_matrix4f(shader_light, "view", view);
        shader_set_matrix4f(shader_light, "projection", projection);
        shader_set_vec3_(shader_light, "lightColor", light_colors[5]);
        model_draw(lamp_back_orange, shader_light);

        shader_set_matrix4f(shader_light, "model", aslk);
        shader_set_matrix4f(shader_light, "view", view);
        shader_set_matrix4f(shader_light, "projection", projection);
        shader_set_vec3_(shader_light, "lightColor", light_colors[4]);
        model_draw(lamp_back_yellow, shader_light);

        shader_use(skybox_shader);
        vvvv[3] = vvvv[7] = vvvv[11] = vvvv[12] = vvvv[13] = vvvv[14] = vvvv[15] = 0.0f;
        shader_set_matrix4f(skybox_shader, "view", vvvv);
        shader_set_matrix4f(skybox_shader, "projection", projection);
        cubemap_skybox_draw(&skybox_item);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        bool horizontal = true, first_iteration = true;
        unsigned int amount = 10;
        shader_use(shader_blur);
        for(unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            shader_set_int(shader_blur, "horizontal", horizontal);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
            bloom_render_quad(&bloom_item);
            horizontal = !horizontal;
            if(first_iteration)
                first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_use(shader_bloom_final);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
        shader_set_int(shader_bloom_final, "bloom", true);
        shader_set_float(shader_bloom_final, "exposure", 1.0f);
        bloom_render_quad(&bloom_item);

        SDL_GL_SwapWindow(window);
    }

    event_destroy(item_event);
    camera_destroy(item_camera);
    free(item_camera);
    context_delete(&gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
