#ifndef BLOOM_INCLUDED
#define BLOOM_INCLUDED
#include "../include/glad/include/glad/glad.h"
#include "../include/GL/gl.h"
#include "../include/stb/stb_image.h"
#include "../header/window.h"
#include <stdlib.h>

struct bloom 
{
    GLuint hdr_FBO;
    GLuint color_buffers[2];
    GLuint rbo_depth;
    GLuint attachments[2];
    GLuint ping_pong_FBO[2];
    GLuint ping_pong_color_buffers[2];
    GLfloat light_positions[4][3];
    GLfloat light_colors[6][3];
    GLuint quad_VAO;
    GLuint quad_VBO;
};

struct bloom *bloom_malloc();
void bloom_init(struct bloom **bloom_item);
void bloom_hdr_buffer(struct bloom **bloom_item);
void bloom_color_buffers(struct bloom **bloom_item);
void bloom_rbo_depth(struct bloom **bloom_item);
void bloom_attachments(struct bloom **bloom_item);
void bloom_ping_pong_buffers(struct bloom **bloom_item);
void bloom_light_colors(struct bloom **bloom_item);
void bloom_light_positions(struct bloom **bloom_item);
void bloom_render_quad(struct bloom **bloom_item);

#endif
