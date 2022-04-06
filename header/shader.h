#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED

#include "../include/glad/glad.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct shader;

struct shader *shader_malloc();

void shader_init(struct shader *, 
                 const char *,
                 const char *,
                 const char *);

void shader_destroy(struct shader *);
void shader_use(struct shader*);
void check_compile_shader(unsigned int,
                          const char *);

void shader_set_matrix4f(struct shader *,
                         const char *,
                         GLfloat *);

void shader_set_vec3(struct shader *,
                     const char *,
                     GLfloat,
                     GLfloat,
                     GLfloat);

unsigned int get_id(struct shader *);

void shader_set_bool(struct shader *,
                     const char *,
                     bool);

#endif
