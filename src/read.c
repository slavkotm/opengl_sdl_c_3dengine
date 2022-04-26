#include "../header/read.h"

struct item 
{
    char data;
    struct item *next;
};

struct item *read_shader_from_file_to_list(const char *filename, const char *flag)
{
    struct item* first = NULL, *last = NULL, *tmp;
    FILE* file;
    file = fopen(filename, flag);
    if(!file)
    {
        perror(filename);
        exit(1);
    }
    while(!feof(file)) 
    {
        if(feof(file))
            break;
        else 
        {
            tmp = malloc(sizeof(struct item));
            fscanf(file, "%c", &tmp->data);
            tmp->next = NULL;
            if(last) 
            {
                last->next = tmp;
                last = last->next;
            } 
            else 
            {
                first = last = tmp;  
            }
        }
    }
    fclose(file);
    return first;
}

const char *get_str_shader(const char *filename, const char *flag)
{
    struct item *list = read_shader_from_file_to_list(filename, flag);
    struct item *copy_list = list;
    int arr_size = list_size(list);
    arr_size++;
    char *arr_symbol = malloc(arr_size * sizeof(char)); 
    int i = 0;
    while(copy_list->next != NULL) {
        arr_symbol[i] = copy_list->data;
        copy_list = copy_list->next;
        i++;
    }
    arr_symbol[i] = '\0';
    list_delete(list);
    return arr_symbol;
}

void list_delete(struct item *list)
{
    while(list != NULL) 
    {
        struct item* tmp = list;
        list = list->next;
        free(tmp);
    }
}

void delete_str_shader(char *str)
{
    free(str);
}

int list_size(struct item *list)
{
    int size = 0;
    while(list != NULL) 
    {
        size++;
        list = list->next;
    }
    return size;
}
