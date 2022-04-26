#include "../header/bloom.h"

struct bloom *bloom_malloc() 
{ 
    return (struct bloom *)malloc(sizeof(struct bloom)); 
};

void bloom_init(struct bloom **bloom_item)
{
    (*bloom_item)->quad_VAO = 0;
};

void bloom_hdr_buffer(struct bloom **bloom_item)
{
    glGenFramebuffers(1, &(*bloom_item)->hdr_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, (*bloom_item)->hdr_FBO);

};

void bloom_color_buffers(struct bloom **bloom_item)
{
    glGenTextures(2, (*bloom_item)->color_buffers);
    for(unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, (*bloom_item)->color_buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, (*bloom_item)->color_buffers[i], 0);
    }
};

void bloom_rbo_depth(struct bloom **bloom_item)
{
    glGenRenderbuffers(1, &(*bloom_item)->rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, (*bloom_item)->rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, (*bloom_item)->rbo_depth);
};

void bloom_attachments(struct bloom **bloom_item)
{
    (*bloom_item)->attachments[0] = GL_COLOR_ATTACHMENT0;
    (*bloom_item)->attachments[1] = GL_COLOR_ATTACHMENT1;
    glDrawBuffers(2, (*bloom_item)->attachments);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("Framebuffer not complete!...\n");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void bloom_ping_pong_buffers(struct bloom **bloom_item)
{
    glGenFramebuffers(2, (*bloom_item)->ping_pong_FBO);
    glGenTextures(2, (*bloom_item)->ping_pong_color_buffers);
    for(unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, (*bloom_item)->ping_pong_FBO[i]);
        glBindTexture(GL_TEXTURE_2D, (*bloom_item)->ping_pong_color_buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, (*bloom_item)->ping_pong_color_buffers[i], 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            printf("Framebuffer not complete!...\n");
    }
};

void bloom_light_colors(struct bloom **bloom_item)
{
    (*bloom_item)->light_colors[0][0] = 5.0f;
    (*bloom_item)->light_colors[0][1] = 5.0f;
    (*bloom_item)->light_colors[0][2] = 5.0f;

    (*bloom_item)->light_colors[1][0] = 2.0f;
    (*bloom_item)->light_colors[1][1] = 0.8f;
    (*bloom_item)->light_colors[1][2] = 1.898f;

    (*bloom_item)->light_colors[2][0] = 0.0f;
    (*bloom_item)->light_colors[2][1] = 0.0f;
    (*bloom_item)->light_colors[2][2] = 15.0f;

    (*bloom_item)->light_colors[3][0] = 0.0f;
    (*bloom_item)->light_colors[3][1] = 2.0f;
    (*bloom_item)->light_colors[3][2] = 2.0f;

    (*bloom_item)->light_colors[4][0] = 5.0f;
    (*bloom_item)->light_colors[4][1] = 1.7f;
    (*bloom_item)->light_colors[4][2] = 1.4f;

    (*bloom_item)->light_colors[5][0] = 5.0f;
    (*bloom_item)->light_colors[5][1] = 0.0f;
    (*bloom_item)->light_colors[5][2] = 0.0f;
};

void bloom_light_positions(struct bloom **bloom_item)
{
    (*bloom_item)->light_positions[0][0] = 0.0f;
    (*bloom_item)->light_positions[0][1] = 0.5f;
    (*bloom_item)->light_positions[0][2] = 1.5f;

    (*bloom_item)->light_positions[1][0] = 0.0f;
    (*bloom_item)->light_positions[1][1] = 3.5f;
    (*bloom_item)->light_positions[1][2] = 13.0f;

    (*bloom_item)->light_positions[2][0] = 3.0f;
    (*bloom_item)->light_positions[2][1] = 0.5f;
    (*bloom_item)->light_positions[2][2] = 1.0f;

    (*bloom_item)->light_positions[3][0] =-0.8f;
    (*bloom_item)->light_positions[3][1] = 2.4f;
    (*bloom_item)->light_positions[3][2] =-1.0f;
};

void bloom_render_quad(struct bloom **bloom_item)
{
     if((*bloom_item)->quad_VAO == 0)
     {
         float quadVertices[] =
         {
             -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
              1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
              1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         };
         glGenVertexArrays(1, &(*bloom_item)->quad_VAO);
         glGenBuffers(1, &(*bloom_item)->quad_VBO);
         glBindVertexArray((*bloom_item)->quad_VAO);
         glBindBuffer(GL_ARRAY_BUFFER, (*bloom_item)->quad_VBO);
         glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
         glEnableVertexAttribArray(0);
         glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
         glEnableVertexAttribArray(1);
         glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
     }
     glBindVertexArray((*bloom_item)->quad_VAO);
     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
     glBindVertexArray(0);
};
