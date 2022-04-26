#ifndef SKYBOX_INCLUDED
#define SKYBOX_INCLUDED
#include "../include/glad/glad.h"
#include "../include/GL/gl.h"
#include "../include/stb/stb_image.h"
#include <stdlib.h>

struct cubemap 
{
    char *skybox[6];
    GLuint VAO;
    GLuint VBO;
    unsigned int cubemap_texture; 
};

struct cubemap *cubemap_malloc();
void cubemap_init(struct cubemap *cubemap_item);
unsigned int cubemap_load(struct cubemap **cubemap_item, unsigned int size);
void cubemap_skybox_draw(struct cubemap **cubemap_item);
void cubemap_mesh(struct cubemap **cubemap_item);

#endif
