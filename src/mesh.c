#include "../header/mesh.h"

char *itoa(int num, char *buffer, int base)
{
    int current = 0;
    if (num == 0) 
    {
        buffer[current++] = '0';
        buffer[current] = '\0';
        return buffer;
    }
    int num_digits = 0;
    if (num < 0) 
    {
        if (base == 10) 
        {
            num_digits ++;
            buffer[current] = '-';
            current++;
            num *= -1;
        }
        else
            return NULL;
    }
    num_digits += (int)floor(log(num) / log(base)) + 1;
    while (current < num_digits)
    {
        int base_val = (int) pow(base, num_digits-1-current);
        int num_val = num / base_val;
        char value = num_val + '0';
        buffer[current] = value;
        current++;
        num -= base_val * num_val;
    }
    buffer[current] = '\0';
    return buffer;
};

struct vertex *vertex_malloc()
{
    return (struct vertex *)malloc(sizeof(struct vertex));
};

struct texture *texture_malloc()
{
    return (struct texture *)malloc(sizeof(struct texture));
};

struct index *index_malloc()
{
    return (struct index *)malloc(sizeof(struct index));
};

struct mesh *mesh_malloc()
{
    return (struct mesh *)malloc(sizeof(struct mesh));
};

void vertex_init(struct vertex *list_vertex)
{
    list_vertex->pos[0] = list_vertex->pos[1] = list_vertex->pos[2] = 0;
    list_vertex->norm[0] = list_vertex->norm[1] = list_vertex->norm[2] = 0;
    list_vertex->texc[0] = list_vertex->texc[1] = 0;
    list_vertex->tang[0] = list_vertex->tang[1] = list_vertex->tang[2] = 0;
    list_vertex->bitang[0] = list_vertex->bitang[1] = list_vertex->bitang[2] = 0;
    list_vertex->next = NULL;
};

void texture_init(struct texture *list_texture, unsigned int item_id, char *item_type, char *item_path)
{
    list_texture->id = item_id;
    list_texture->type = item_type;
    list_texture->path = item_path;
    list_texture->next = NULL;
};

void index_init(struct index *list_index, unsigned int item_data)
{
    list_index->data = item_data;
    list_index->next = NULL;
};

struct mesh *mesh_init(struct mesh *list_mesh, struct vertex *list_vertex, struct texture *list_texture, struct index *list_index)
{
    list_mesh->list_vertex = list_vertex;
    list_mesh->list_texture = list_texture;
    list_mesh->list_index = list_index;
    list_mesh->next = NULL;
    return list_mesh;
};

void vertex_push_back(struct vertex **list_vertex, struct vertex *item_vertex)
{
    struct vertex *tmp_vertex = NULL;
    if(*list_vertex == NULL)
    {
        (*list_vertex) = malloc(sizeof(struct vertex));

        (*list_vertex)->pos[0] = item_vertex->pos[0];
        (*list_vertex)->pos[1] = item_vertex->pos[1];
        (*list_vertex)->pos[2] = item_vertex->pos[2];

        (*list_vertex)->norm[0] = item_vertex->norm[0];
        (*list_vertex)->norm[1] = item_vertex->norm[1];
        (*list_vertex)->norm[2] = item_vertex->norm[2];

        (*list_vertex)->texc[0] = item_vertex->texc[0];
        (*list_vertex)->texc[1] = item_vertex->texc[1];

        (*list_vertex)->tang[0] = item_vertex->tang[0];
        (*list_vertex)->tang[1] = item_vertex->tang[1];
        (*list_vertex)->tang[2] = item_vertex->tang[2];

        (*list_vertex)->bitang[0] = item_vertex->bitang[0];
        (*list_vertex)->bitang[1] = item_vertex->bitang[1];
        (*list_vertex)->bitang[2] = item_vertex->bitang[2];

        (*list_vertex)->next = NULL;
    }
    else if(*list_vertex != NULL)
    {
        while((*list_vertex)->next != NULL)
            (*list_vertex) = (*list_vertex)->next;

        tmp_vertex = malloc(sizeof(struct vertex));

        tmp_vertex->pos[0] = item_vertex->pos[0];
        tmp_vertex->pos[1] = item_vertex->pos[1];
        tmp_vertex->pos[2] = item_vertex->pos[2];

        tmp_vertex->norm[0] = item_vertex->norm[0];
        tmp_vertex->norm[1] = item_vertex->norm[1];
        tmp_vertex->norm[2] = item_vertex->norm[2];

        tmp_vertex->texc[0] = item_vertex->texc[0];
        tmp_vertex->texc[1] = item_vertex->texc[1];

        tmp_vertex->tang[0] = item_vertex->tang[0];
        tmp_vertex->tang[1] = item_vertex->tang[1];
        tmp_vertex->tang[2] = item_vertex->tang[2];

        tmp_vertex->bitang[0] = item_vertex->bitang[0];
        tmp_vertex->bitang[1] = item_vertex->bitang[1];
        tmp_vertex->bitang[2] = item_vertex->bitang[2];

        tmp_vertex->next = NULL;

        (*list_vertex)->next = tmp_vertex;
    }
};

void texture_item_push_back(struct texture **list_texture, struct texture* item_texture)
{
    if(*list_texture == NULL)
    {
        (*list_texture) = item_texture;
    }
    else if(*list_texture != NULL)
    {
        while((*list_texture)->next != NULL)
            (*list_texture) = (*list_texture)->next;
        (*list_texture)->next = item_texture;
    }
};

void texture_push_back(struct texture **list_texture, unsigned int item_id, char *item_type, char *item_path)
{
    struct texture *tmp_texture = NULL;
    if(*list_texture == NULL)
    {
        (*list_texture) = (struct texture *)malloc(sizeof(struct texture));
        (*list_texture)->id = item_id;
        (*list_texture)->path = item_path;
        (*list_texture)->type = item_type;
        (*list_texture)->next = NULL;
    }
    else if(*list_texture != NULL)
    {
        while((*list_texture)->next != NULL)
            (*list_texture) = (*list_texture)->next;

        tmp_texture = (struct texture *)malloc(sizeof(struct texture));
        tmp_texture->id = item_id;
        tmp_texture->path = item_path;
        tmp_texture->type = item_type;
        tmp_texture->next = NULL;
        (*list_texture)->next = tmp_texture;
    }
};

void index_push_back(struct index **list_index, unsigned int item_data)
{
    struct index *tmp_index = NULL;
    if(*list_index == NULL)
    {
        *list_index = (struct index *)malloc(sizeof(struct index));
        (*list_index)->data = item_data;
        (*list_index)->next = NULL;
    }
    else if(list_index != NULL)
    {
        while((*list_index)->next != NULL)
            (*list_index) = (*list_index)->next;

        tmp_index = (struct index *)malloc(sizeof(struct index));
        tmp_index->data = item_data;
        tmp_index->next = NULL;
        (*list_index)->next = tmp_index;
    }
};

void mesh_push_back(struct mesh *list_mesh, struct mesh *item_mesh)
{
    struct mesh *tmp_mesh = NULL, *tmp_list_mesh = list_mesh;
    if(list_mesh == NULL)
    {
        list_mesh = (struct mesh *)malloc(sizeof(struct mesh));
        list_mesh->list_vertex = item_mesh->list_vertex;
        list_mesh->list_texture = item_mesh->list_texture;
        list_mesh->list_index = item_mesh->list_index;
        list_mesh->VAO = item_mesh->VAO;
        list_mesh->VBO = item_mesh->VBO;
        list_mesh->EBO = item_mesh->EBO;
        list_mesh->next = NULL;
    }
    else if(list_mesh != NULL)
    {
        while(tmp_list_mesh->next != NULL)
            tmp_list_mesh = tmp_list_mesh->next;

        tmp_mesh = (struct mesh *)malloc(sizeof(struct mesh));    
        tmp_mesh->list_vertex = item_mesh->list_vertex;
        tmp_mesh->list_texture = item_mesh->list_texture;
        tmp_mesh->list_index = item_mesh->list_index;
        tmp_mesh->VAO = item_mesh->VAO;
        tmp_mesh->VBO = item_mesh->VBO;
        tmp_mesh->EBO = item_mesh->EBO;
        tmp_mesh->next = NULL;
        tmp_list_mesh->next = tmp_mesh;
    }
};

void mesh_push_back_item_mesh(struct mesh **list_mesh, struct mesh *item_mesh)
{    
    struct mesh *tmp_mesh = NULL;
    if((*list_mesh) == NULL)
    {
        (*list_mesh) = (struct mesh *)malloc(sizeof(struct mesh));
        (*list_mesh)->list_vertex = item_mesh->list_vertex;
        (*list_mesh)->list_texture = item_mesh->list_texture;
        (*list_mesh)->list_index = item_mesh->list_index;
        (*list_mesh)->next = NULL;
        mesh_setup(list_mesh);
    }
    else if((*list_mesh) != NULL)
    {
        while((*list_mesh)->next != NULL)
            (*list_mesh) = (*list_mesh)->next;

        tmp_mesh = (struct mesh *)malloc(sizeof(struct mesh));
        tmp_mesh->list_vertex = item_mesh->list_vertex;
        tmp_mesh->list_texture = item_mesh->list_texture;
        tmp_mesh->list_index = item_mesh->list_index;
        tmp_mesh->next = NULL;
        (*list_mesh)->next = tmp_mesh;
        mesh_setup(&tmp_mesh);
    }
};

int vertex_list_size(struct vertex *list_vertex)
{
    int size = 0;
        while(list_vertex != NULL)
        {
            list_vertex = list_vertex->next;
            size++;
        }
    return size;
};

int texture_list_size(struct texture *list_texture)
{
    int size = 0;
        while(list_texture != NULL)
        {
            list_texture = list_texture->next;
            size++;
        }
    return size;
};

int index_list_size(struct index *list_index)
{
    int size = 0;
        while(list_index != NULL)
        {
            list_index = list_index->next;
            size++;
        }
    return size;
};

struct texture *texture_get_path_item_by_index(struct texture *list_texture, int index)
{
    int i = 0;
        while(i < index && list_texture != NULL)
        {
            list_texture = list_texture->next;
            i++;
        }
    return list_texture;
};

int mesh_list_size(struct mesh *list_mesh)
{
    int size = 0;
        while(list_mesh != NULL)
        {
            list_mesh = list_mesh->next;
            size++;
        }
    return size;
};

char *texture_get_path_by_index(struct texture *list_texture, int index)
{
    int i = 0; 
    while(i < index && list_texture != NULL)
    {
        list_texture = list_texture->next;
        i++;
    }
    return list_texture->path;
};

char *texture_get_type_by_index(struct texture *list_texture, int index)
{
    int i = 0; 
    while(i < index && list_texture != NULL)
    {
        list_texture = list_texture->next;
        i++;
    }
    return list_texture->type;
};

unsigned int texture_get_id_by_index(struct texture *list_texture, int index)
{
    int i = 0; 
    while(i < index && list_texture != NULL)
    {
        list_texture = list_texture->next;
        i++;
    }
    return list_texture->id;
};

struct mesh *mesh_get_item_by_index(struct mesh *list_mesh, int index)
{
    int i = 0; 
    while(i < index && list_mesh != NULL)
    {
        list_mesh = list_mesh->next;
        i++;
    }

    return list_mesh;
};

void mesh_setup(struct mesh **list_mesh)
{
    int size_index = index_list_size((*list_mesh)->list_index);
    int size_vertex = vertex_list_size((*list_mesh)->list_vertex) * 14;

    GLfloat *arr_vertex = malloc(size_vertex * sizeof(GLfloat));
    unsigned int *arr_index = malloc(size_index * sizeof(unsigned int));

    struct vertex *tmp_vertex = (*list_mesh)->list_vertex;
    struct index *tmp_index = (*list_mesh)->list_index;

    unsigned int i = 0;

    while(tmp_vertex != NULL)
    {
        arr_vertex[i] = tmp_vertex->pos[0];
        i++;
        arr_vertex[i] = tmp_vertex->pos[1];
        i++;
        arr_vertex[i] = tmp_vertex->pos[2];
        i++;

        arr_vertex[i] = tmp_vertex->norm[0];
        i++;
        arr_vertex[i] = tmp_vertex->norm[1];
        i++;
        arr_vertex[i] = tmp_vertex->norm[2];
        i++;

        arr_vertex[i] = tmp_vertex->texc[0];
        i++;
        arr_vertex[i] = tmp_vertex->texc[1];
        i++;

        arr_vertex[i] = tmp_vertex->tang[0];
        i++;
        arr_vertex[i] = tmp_vertex->tang[1];
        i++;
        arr_vertex[i] = tmp_vertex->tang[2];
        i++;
        
        arr_vertex[i] = tmp_vertex->bitang[0];
        i++;
        arr_vertex[i] = tmp_vertex->bitang[1];
        i++;
        arr_vertex[i] = tmp_vertex->bitang[2];
        i++;

        tmp_vertex = tmp_vertex->next;
    }

    unsigned int j = 0;
    while(tmp_index != NULL)
    {
        arr_index[j] = tmp_index->data;
        tmp_index = tmp_index->next;
        j++;
    }

    glGenVertexArrays(1, &(*(*list_mesh)).VAO);
    glGenBuffers(1, &(*(*list_mesh)).VBO);
    glGenBuffers(1, &(*(*list_mesh)).EBO);

    glBindVertexArray((*(*list_mesh)).VAO);
    glBindBuffer(GL_ARRAY_BUFFER, (*(*list_mesh)).VBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*(*list_mesh)).EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_index * sizeof(unsigned int), arr_index, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, size_vertex * sizeof(GLfloat), arr_vertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void *)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (void *)(11 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
};

void mesh_draw(struct mesh *list_mesh, struct shader *item_shader)
{
    unsigned int diffuse_nr = 1;
    unsigned int specular_nr = 1;
    unsigned int normal_nr = 1;
    unsigned int height_nr = 1;
    for(unsigned int i = 0; i < texture_list_size(list_mesh->list_texture); i++)
    {
        glad_glActiveTexture(GL_TEXTURE0 + i);

        char number[1024];
        char *name = texture_get_type_by_index(list_mesh->list_texture, i);
        if(strcmp(name, "texture_diffuse") == 0)
        {
            itoa(diffuse_nr, number, 10);
            diffuse_nr++;
        }
        else if(strcmp(name, "texture_specular") == 0)
        {
            itoa(specular_nr, number, 10);
            specular_nr++;
        }
        else if(strcmp(name, "texture_normal") == 0)
        {
            itoa(normal_nr, number, 10);
            normal_nr++;
        }
        else if(strcmp(name, "texture_height") == 0)
        {
            itoa(height_nr, number, 10);
            height_nr++;
        }

        int index = 0;
        while(name[index] != '\0')
            index++;
        char last_name[index];
        for(int i = 0; i < index; i++)
            last_name[i] = name[i];

        glad_glUniform1i(glad_glGetUniformLocation(get_id(item_shader), strcat(last_name, number)), i);
        glad_glBindTexture(GL_TEXTURE_2D, texture_get_id_by_index(list_mesh->list_texture, i));
    }

    glBindVertexArray((*list_mesh).VAO);
    glDrawElements(GL_TRIANGLES,  index_list_size((*list_mesh).list_index), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glad_glActiveTexture(GL_TEXTURE0);
};

void vertex_list_destroy(struct vertex *list_vertex);
void texture_list_destroy(struct texture *list_texture);
void index_list_destroy(struct index *list_index);
void mesh_list_destroy(struct mesh *list_mesh);
