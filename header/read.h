#ifndef LIST_INCLUDED
#define LIST_INCLUDED

#include <stdlib.h>
#include <stdio.h>

struct item; 
struct item *read_shader_from_file_to_list(const char *filename, const char *flag);
const char *get_str_shader(const char *filename, const char *flag);
int list_size(struct item *list);
void delete_str_shader(char *str);
void list_delete(struct item *list);

#endif
