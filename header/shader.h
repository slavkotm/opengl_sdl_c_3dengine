#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED
#include "../include/glad/glad.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct shader;
struct shader *shader_malloc();
void shader_init(struct shader *item_shader, const char *str_vertex_shader, const char *str_fragment_shader, const char *flag);
void shader_destroy(struct shader *item_shader);
void shader_use(struct shader *item_shader);
void check_compile_shader(unsigned int shader, const char *type);
void shader_set_matrix4f(struct shader *item_shader, const char *shader_name, GLfloat *matrix);
void shader_set_float(struct shader *item_shader, const char *shader_name, GLfloat value);
void shader_set_int(struct shader *item_shader, const char *shader_name, int value);
void shader_set_vec3(struct shader *item_shader, const char *shader_name, GLfloat x, GLfloat y, GLfloat z);
void shader_set_vec3_(struct shader *item_shader, const char *shader_name, GLfloat *vec);
unsigned int get_id(struct shader *item_shader);
void shader_set_bool(struct shader *item_shader, const char *shader_name, bool value);

#endif
