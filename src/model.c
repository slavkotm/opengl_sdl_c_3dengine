#include "../header/model.h"

char *model_substr(const char *path)
{
    int size = 0;
    while(path[size] != '\0')
        size++;
    while(path[size] != '/')
        size--;
    char *str = malloc((size + 1) * sizeof(char));
    for(int i = 0; i < size; i++)
        str[i] = path[i];
    str[size] = '\0';
    return str;
};

struct model *model_malloc()
{
    return (struct model *)malloc(sizeof(struct model));
};

void model_init(struct model *item_model, struct mesh *item_mesh, char *path, bool item_gamma_correction)
{
    item_model->meshes = NULL;
    item_model->copy_meshes = NULL;
    item_model->flag = true;
    item_model->f1 = true;
    item_model->textures_loaded = NULL;
    model_load(item_model, path, item_gamma_correction);
};

void model_draw(struct model *item_model, struct shader *item_shader)
{
    for(unsigned int i = 0; i < mesh_list_size(item_model->copy_meshes); i++)
    {
        struct mesh *tmp = mesh_get_item_by_index(item_model->copy_meshes, i);
        mesh_draw(tmp, item_shader);
    }
};

char *str_cat(char *s1, char *s2)
{
    int size_s1 = strlen(s1), size_s2 = strlen(s2);
    char *s3 = malloc((size_s2 + size_s1 + 1) * sizeof(char));
    int i = 0;
    while(s1[i] != '\0')
    {
        s3[i] = s1[i]; i++;
    }
    int j = 0;
    while(s2[j] != '\0')
    {
        s3[i] = s2[j]; j++; i++;
    }
    s3[i] = '\0';
    return s3;
}

unsigned int model_texture_from_file(const char *path,
                                     char *directory,
                                     bool gamma)
{
    char *file_name = (char *)path;
    file_name = str_cat(directory, "/");
    file_name = str_cat(file_name, (char *)path);

    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width, height, nr_components;
    printf("%s\n", file_name);
    unsigned char *data = stbi_load(file_name, &width, &height, &nr_components, 0);
    if(data)
    {
        GLenum format;
        if(nr_components == 1)
            format = GL_RED;
        else if(nr_components == 3)
            format = GL_RGB;
        else if(nr_components == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else 
    {
        printf("Texture failed to load at path: %s\n", path);
        stbi_image_free(data);
    }
    
    free(file_name);
    file_name = NULL;

    return texture_id;
};

struct texture *model_load_material_textures(struct model *item_model, struct aiMaterial *mat, enum aiTextureType type, char *type_name)
{
    struct texture *textures = NULL, *tmp_textures = NULL, *tmp_texture_loaded = NULL;
    bool f1 = true, f2 = true, f3 = true, f4 = true;

    for(unsigned int i = 0; i < aiGetMaterialTextureCount(mat, type); i++)
    {

        struct aiString str;
        aiGetMaterialTexture(mat, type, i, &str, NULL, NULL, NULL, NULL, NULL, NULL);

        bool skip = false;
        
        for(unsigned int j = 0; j < texture_list_size(item_model->textures_loaded); j++)
        {
            if(strcmp(texture_get_type_by_index(item_model->textures_loaded, j), str.data) == 0)
            {
                struct texture *tmp = texture_get_path_item_by_index(item_model->textures_loaded, j);
                if(f1)
                {
                    texture_push_back(&textures, tmp->id, tmp->type, tmp->path);
                    f1 = false;
                }
                else 
                {
                    texture_push_back(&tmp_textures, tmp->id, tmp->type, tmp->path);
                    if(f2)
                    {
                        textures->next = tmp_textures;
                        f2 = false;
                    }
                }

                skip = true;
                break;
            }
        }

        if(!skip)
        {
            struct texture textr;
            textr.id = model_texture_from_file(str.data, item_model->directory, false);
            textr.type = type_name;
            textr.path = str.data;
            texture_push_back(&textures, textr.id, textr.type, textr.path);
            if(f3)
            {
                texture_push_back(&item_model->textures_loaded, textr.id, textr.type, textr.path);
                f3 = false;
            }
            else 
            {
                texture_push_back(&tmp_texture_loaded, textr.id, textr.type, textr.path);
                if(f4)
                {
                    item_model->textures_loaded->next = tmp_texture_loaded;
                    f4 = false;
                }
            }
        }
    }
    printf("model_load_material_textures\n");

    return textures;
};

struct mesh *model_process_mesh(struct model *item_model, struct aiMesh *mesh, const struct aiScene *scene)
{
    struct vertex *list_vertex = NULL, *copy_list_vertex = NULL;
    struct index *list_index = NULL, *copy_list_index = NULL;
    struct texture *list_texture = NULL, *copy_list_texture = NULL;
    bool f1 = true,f2 = true, f3 = true;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        struct vertex vert;
        vert.pos[0] = mesh->mVertices[i].x;
        vert.pos[1] = mesh->mVertices[i].y;
        vert.pos[2] = mesh->mVertices[i].z;

        vert.norm[0] = mesh->mNormals[i].x;
        vert.norm[1] = mesh->mNormals[i].y;
        vert.norm[2] = mesh->mNormals[i].z;

        if(mesh->mTextureCoords[0])
        {
            vert.texc[0] = mesh->mTextureCoords[0][i].x;
            vert.texc[1] = mesh->mTextureCoords[0][i].y;
        }
        else 
        {
            vert.texc[0] = 0;
            vert.texc[1] = 0;
        }

        vert.tang[0] = mesh->mTangents[i].x;
        vert.tang[1] = mesh->mTangents[i].y;
        vert.tang[2] = mesh->mTangents[i].z;

        vert.bitang[0] = mesh->mBitangents[i].x;
        vert.bitang[1] = mesh->mBitangents[i].y;
        vert.bitang[2] = mesh->mBitangents[i].z;

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

    struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    
printf("ZADONTA' ESLI NE PIDOR\n");
    struct texture *diffuse_maps = model_load_material_textures(item_model, material, aiTextureType_DIFFUSE, "texture_diffuse");
    texture_item_push_back(&copy_list_texture, diffuse_maps);

    struct texture *specular_maps = model_load_material_textures(item_model, material, aiTextureType_SPECULAR, "texture_specular");
    texture_item_push_back(&list_texture, specular_maps);
    printf("ИХХИХИХИХИХХИИХИХХИХИХИХИХИХИ           %d\n", texture_list_size(copy_list_texture));
    if(copy_list_texture != NULL)
        copy_list_texture->next = list_texture;

    struct texture *normal_maps = model_load_material_textures(item_model, material, aiTextureType_HEIGHT, "texture_normal");
    texture_item_push_back(&list_texture, normal_maps);

    struct texture *height_maps = model_load_material_textures(item_model, material, aiTextureType_AMBIENT, "texture_height");
    texture_item_push_back(&list_texture, height_maps);

    struct mesh *list_mesh = mesh_malloc();
    return mesh_init(list_mesh, copy_list_vertex, copy_list_texture, copy_list_index);
};

void model_process_node(struct model *item_model, struct aiNode *node, const struct aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        struct mesh *mesh_tmp = model_process_mesh(item_model, mesh, scene);
        if(item_model->flag)
        {
            mesh_push_back_item_mesh(&item_model->copy_meshes, mesh_tmp);
            item_model->flag = false;
        }
        else
        {
            mesh_push_back_item_mesh(&item_model->meshes, mesh_tmp);
            if(item_model->f1)
            {
                item_model->copy_meshes->next = item_model->meshes;
                item_model->f1 = false;
            }
        }
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
        model_process_node(item_model, node->mChildren[i], scene);
};

void model_load(struct model *item_model, const char *path, bool isUV_flipped)
{
    const struct aiScene *scene = NULL;
    if(isUV_flipped)
        scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    else 
        scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
        return;
    }


    item_model->directory = model_substr(path);
    printf("%s\n", item_model->directory);

    model_process_node(item_model, scene->mRootNode, scene);

    aiReleaseImport(scene);
};

