#ifndef MESH_INCLUDED
#define MESH_INCLUDED
#include "../include/glad/glad.h"
#include "../include/GL/gl.h"
#include "../header/shader.h"
#include "../include/gsl/gsl_vector.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

char *itoa(int num,
                  char *buffer,
                  int base);


struct vertex
{ 
    float pos[3];
    float norm[3];
    float texc[2];
    float tang[3];
    float bitang[3];
    struct vertex *next;
};

struct texture
{
    unsigned int id;
    char *type;
    char *path;
    struct texture *next;
};
    
struct index
{
    unsigned int data;
    struct index *next;
};

struct mesh 
{
    struct vertex *list_vertex;
    struct index *list_index;
    struct texture *list_texture;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int VBO;
    struct mesh *next;
};



struct vertex *vertex_malloc();
struct texture *texture_malloc();
struct index *index_malloc();
struct mesh *mesh_malloc();

void vertex_init(struct vertex *list_vertex);
void texture_init(struct texture *list_texture,
                  unsigned int item_id,
                  char *item_type,
                  char *item_path);
void index_init(struct index *list_index,
                unsigned int item_data);
struct mesh *mesh_init(struct mesh *list_mesh,
                      struct vertex *list_vertex,
                      struct texture *list_texture,
                      struct index *list_index);

void vertex_push_back(struct vertex **list_vertex,
                      struct vertex *item_vertex);
void texture_push_back(struct texture **list_texture,
                       unsigned int item_id,
                       char *item_type,
                       char *item_path);
void index_push_back(struct index **list_index,
                     unsigned int item_data);
void mesh_push_back(struct mesh *list_mesh,
                    struct mesh *item_mesh);
void mesh_push_back_item_mesh(struct mesh **list_mesh,
                              struct mesh *item_mesh);

int vertex_list_size(struct vertex *list_vertex);
int texture_list_size(struct texture *list_texture);
int index_list_size(struct index *list_index);
int mesh_list_size(struct mesh *list_mesh);

void texture_item_push_back(struct texture **list_texture, struct texture* item_texture);
struct texture *texture_get_path_item_by_index(struct texture *list_texture, int index);
char *texture_get_path_by_index(struct texture *list_texture, int index);
char *texture_get_type_by_index(struct texture *list_texture,
                                int index);
unsigned int texture_get_id_by_index(struct texture *list_texture,
                                     int index);

void mesh_setup(struct mesh **list_mesh);
void mesh_draw(struct mesh *list_mesh,
               struct shader *item_shader);
struct mesh *mesh_get_item_by_index(struct mesh *list_mesh,
                                    int index);

void vertex_list_destroy(struct vertex *list_vertex);
void texture_list_destroy(struct texture *list_texture);
void index_list_destroy(struct index *list_index);
void mesh_list_destroy(struct mesh *list_mesh);

#endif
