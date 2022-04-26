#include "../header/skybox.h"

struct cubemap *cubemap_malloc()
{
    return (struct cubemap *)malloc(sizeof(struct cubemap));
};

void cubemap_init(struct cubemap *cubemap_item)
{
    cubemap_item->skybox[2] = "assets/skybox/top.jpg";
    cubemap_item->skybox[5] = "assets/skybox/back.jpg";
    cubemap_item->skybox[1] = "assets/skybox/left.jpg";
    cubemap_item->skybox[4] = "assets/skybox/front.jpg";
    cubemap_item->skybox[0] = "assets/skybox/right.jpg";
    cubemap_item->skybox[3] = "assets/skybox/bottom.jpg";
};

unsigned int cubemap_load(struct cubemap **cubemap_item, unsigned int size)
{
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width, height, nr_channels;
    for(unsigned int i = 0; i < size; i++)
    {
        unsigned char *data = stbi_load((*cubemap_item)->skybox[i], &width, &height, &nr_channels, 0);
        if(data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else 
        {
            printf("Cubemap texture failed to load at path: %s\n", (*cubemap_item)->skybox[i]);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);     
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    (*cubemap_item)->cubemap_texture = texture_id;

    return texture_id;
};

void cubemap_skybox_draw(struct cubemap **cubemap_item)
{
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray((*cubemap_item)->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, (*cubemap_item)->cubemap_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
};

void cubemap_mesh(struct cubemap **cubemap_item)
{
    float skybox_vertices[] = {

        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &(*cubemap_item)->VAO);
    glGenBuffers(1, &(*cubemap_item)->VBO);
    glBindVertexArray((*cubemap_item)->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, (*cubemap_item)->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
};
