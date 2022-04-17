#include "../header/model.h"

char *model_substr(const char *path)
{
    int size = 0;
    while(path[size] != '0')
        size++;
    while(path[size] != '/')
        size--;
    char *str = malloc((size + 1) * sizeof(char));
    for(int i = 0; i < size + 1; i++)
        str[i] = path[i];
    return str;
};

struct model *model_malloc()
{
    return (struct model *)malloc(sizeof(struct model));
};

void model_init(struct model *item_model,
                struct mesh *item_mesh,
                char *path,
                bool item_gamma_correction)
{
    item_model->meshes = NULL;
    item_model->copy_meshes = NULL;
    item_model->flag = true;
    item_model->f1 = true;
    item_model->ll = true;
    item_model->textures_loaded = NULL;
    model_load(item_model, path);
};

void model_draw(struct model *item_model,
                struct shader *item_shader)
{
    for(unsigned int i = 0; i < mesh_list_size(item_model->copy_meshes); i++)
    {
    printf("SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS%d\n", mesh_list_size(item_model->copy_meshes));
        struct mesh *tmp = mesh_get_item_by_index(item_model->copy_meshes, i);
        printf("LOHOHOHOHOHOHO%d iiiiiiiiiiiiiiiiii %d\n", tmp->VAO, i);
        mesh_draw(tmp, item_shader);
    }
    
};

struct mesh *model_process_mesh(struct aiMesh *mesh,
                               const struct aiScene *scene)
{
    printf("model_process_mesh\n");
    struct vertex *list_vertex = NULL, *copy_list_vertex = NULL;
    struct index *list_index = NULL, *copy_list_index = NULL;
    struct texture *list_texture = NULL;
    bool f1 = true,f2 = true, f3 = true;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        struct vertex vert ;
        
/*        vert->position = gsl_vector_alloc(3);
        gsl_vector_set(vert->position, 0, mesh->mVertices[i].x);
        gsl_vector_set(vert->position, 1, mesh->mVertices[i].y);
        gsl_vector_set(vert->position, 2, mesh->mVertices[i].z);*/

        vert.pos[0] = mesh->mVertices[i].x;
        vert.pos[1] = mesh->mVertices[i].y;
        vert.pos[2] = mesh->mVertices[i].z;

/*        vert->normal = gsl_vector_alloc(3);
        gsl_vector_set(vert->normal, 0, mesh->mNormals[i].x);
        gsl_vector_set(vert->normal, 1, mesh->mNormals[i].y);
        gsl_vector_set(vert->normal, 2, mesh->mNormals[i].z);*/

/*        vert->norm[0] = mesh->mNormals[i].x;
        vert->norm[1] = mesh->mNormals[i].y;
        vert->norm[2] = mesh->mNormals[i].z;*/

        if(mesh->mTextureCoords[0])
        {
/*            vert->texcoords = gsl_vector_alloc(2);
            gsl_vector_set(vert->texcoords, 0, mesh->mTextureCoords[0][i].x);
            gsl_vector_set(vert->texcoords, 1, mesh->mTextureCoords[0][i].y);*/

/*            vert->texc[0] = mesh->mTextureCoords[0][i].x;
            vert->texc[1] = mesh->mTextureCoords[0][i].y;*/
        }
        else 
        {
/*            gsl_vector_set(vert->texcoords, 0, 0);
            gsl_vector_set(vert->texcoords, 1, 0);*/

/*            vert->texc[0] = 0;
            vert->texc[1] = 0;*/
        }

/*        vert->tangent = gsl_vector_alloc(3);
        gsl_vector_set(vert->tangent, 0, mesh->mTangents[i].x);
        gsl_vector_set(vert->tangent, 1, mesh->mTangents[i].y);
        gsl_vector_set(vert->tangent, 2, mesh->mTangents[i].z);*/

/*        vert->tang[0] = mesh->mTangents[i].x;
        vert->tang[1] = mesh->mTangents[i].y;
        vert->tang[2] = mesh->mTangents[i].z;*/

/*        vert->bitangent = gsl_vector_alloc(3);
        gsl_vector_set(vert->bitangent, 0, mesh->mBitangents[i].x);
        gsl_vector_set(vert->bitangent, 1, mesh->mBitangents[i].y);
        gsl_vector_set(vert->bitangent, 2, mesh->mBitangents[i].z);*/

/*        vert->bitang[0] = mesh->mBitangents[i].x;
        vert->bitang[1] = mesh->mBitangents[i].y;
        vert->bitang[2] = mesh->mBitangents[i].z;*/

        if(f1)
        {
            vertex_push_back(&copy_list_vertex, &vert);
            f1 = false;
        }
        else 
        {
            vertex_push_back(&list_vertex, &vert);
            if(f3)
            {
                copy_list_vertex->next = list_vertex;
                f3 = false;
            }
        }
    }

    printf("vertex_count    %d\n", vertex_list_size(copy_list_vertex));
    printf("x = %.6lf   y = %.6lf    z = %.6lf\n",
            copy_list_vertex->pos[0], copy_list_vertex->pos[1],
            copy_list_vertex->pos[2]);
    printf("vertex_count    %d\n", vertex_list_size(copy_list_vertex));
    printf("x = %.6lf   y = %.6lf    z = %.6lf\n",
            copy_list_vertex->next->pos[0], copy_list_vertex->next->pos[1],
            copy_list_vertex->next->pos[2]);
    bool f4 = true;
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        struct aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            if(f2)
            {
                index_push_back(&copy_list_index, face.mIndices[j]);
                list_index = copy_list_index;
                f2 = false;
            }
            else 
                index_push_back(&list_index, face.mIndices[j]);

        }
    }
    printf("x = %d   y = %d    z = %d\n",
            copy_list_index->data, copy_list_index->next->data,
            copy_list_index->next->next->data);

    printf("index_count   %d\n", index_list_size(copy_list_index));

    struct mesh *list_mesh = mesh_malloc();
    return mesh_init(list_mesh, copy_list_vertex, list_texture, copy_list_index);
};


struct mesh *get_addres(struct mesh *item_mesh)
{
    struct mesh *result = item_mesh;
    return result;
};

void model_process_node(struct model *item_model,
                        struct aiNode *node,
                        const struct aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        struct mesh *mesh_tmp = model_process_mesh(mesh, scene);
        if(item_model->flag)
        {
            mesh_push_back_item_mesh(&item_model->copy_meshes, mesh_tmp);
            item_model->flag = false;
        printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR%d\n",
                item_model->copy_meshes->VAO);
           
        }
        else
        {
            mesh_push_back_item_mesh(&item_model->meshes, mesh_tmp);
            if(item_model->f1)
            {
                item_model->copy_meshes->next = item_model->meshes;
                item_model->f1 = false;
            }
        printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR%d\n",
                item_model->meshes->VAO);
        }
        


    }
    if(item_model->ll == true)
    {
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            model_process_node(item_model, node->mChildren[i], scene);
            //if(i == 2)
            //{
              //  item_model->ll = false;
                //break;
            //}
        }
    }
};

void model_load(struct model *item_model,
                const char *path)
{
    const struct aiScene *scene = aiImportFile(path, 
                                               aiProcess_CalcTangentSpace |
                                               aiProcess_Triangulate |
                                               aiProcess_FlipUVs
                                               );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
        return;
    }


    item_model->directory = model_substr(path);

    model_process_node(item_model, scene->mRootNode, scene);

    aiReleaseImport(scene);
};

