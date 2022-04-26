#ifndef MODEL_INLCUDED
#define MODEL_INLCUDED
#include "../header/mesh.h"
#include "../include/assimp/cimport.h"
#include "../include/assimp/scene.h"
#include "../include/assimp/postprocess.h"
#include "../include/stb/stb_image.h"
#include <stdbool.h>

struct model 
{
    struct texture *textures_loaded;
    struct mesh *meshes;
    struct mesh *copy_meshes;
    struct mesh *last;
    bool flag;
    bool f1;
    char *directory;
    bool gamma_correction;
};

struct model *model_malloc();

void model_init(struct model *item_model,
                struct mesh *item_mesh,
                char *path,
                bool item_gamma_correction);

void model_draw(struct model *item_model,
                struct shader *item_shader);

void model_load(struct model *item_model,
                const char *path,
                bool isUV_flipped);

void model_process_node(struct model *item_model, 
                        struct aiNode *node,
                        const struct aiScene *scene);

struct mesh *model_process_mesh(struct model *item_model,
                                struct aiMesh *mesh,
                                const struct aiScene *scene);

struct texture *model_load_material_textures(struct model *item_model,
                                             struct aiMaterial *mat,
                                             enum aiTextureType type,
                                             char *type_name);

unsigned int model_texture_from_file(const char *path,
                                     char *directory,
                                     bool gamma);

char *model_substr(const char *path);
char *str_cat(char *s1, char *s2);

#endif
