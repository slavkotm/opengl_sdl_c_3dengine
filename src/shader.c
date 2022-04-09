#include "../header/shader.h"
#include "../header/read.h"

struct shader
{
    unsigned int id;
};

void shader_destroy(struct shader *item_shader) { free(item_shader); };

unsigned int get_id(struct shader *item_shader) { return item_shader->id; };

void shader_use(struct shader *item_shader) { glad_glUseProgram(item_shader->id); };

struct shader* shader_malloc() { return (struct shader*)malloc(sizeof(struct shader)); };

void shader_init(struct shader *item_shader,
                 const char *str_vertex_shader,
                 const char *str_fragment_shader,
                 const char *flag)
{
    /*vertex shader*/
    const GLchar *vertex_shader_source = get_str_shader(str_vertex_shader, flag); 
    GLuint vertex_shader;
    vertex_shader = glad_glCreateShader(GL_VERTEX_SHADER);
    glad_glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glad_glCompileShader(vertex_shader);
    check_compile_shader(vertex_shader, "VERTEX");

    /*fragment shader*/
    const GLchar *fragment_shader_source = get_str_shader(str_fragment_shader, 
                                                          flag);
    GLuint fragment_shader;
    fragment_shader = glad_glCreateShader(GL_FRAGMENT_SHADER);
    glad_glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glad_glCompileShader(fragment_shader);
    check_compile_shader(fragment_shader, "FRAGMENT");

    /*shader program*/
    item_shader->id = glad_glCreateProgram();
    glad_glAttachShader(item_shader->id, vertex_shader);
    glad_glAttachShader(item_shader->id, fragment_shader);
    glad_glLinkProgram(item_shader->id);
    check_compile_shader(item_shader->id, "PROGRAM");

    /*delete shaders*/
    glad_glDeleteShader(vertex_shader);
    glad_glDeleteShader(fragment_shader);

    delete_str_shader((char *)vertex_shader_source);
    delete_str_shader((char *)fragment_shader_source);
}

void check_compile_shader(unsigned int shader,
                          const char *type)
{
    int success;
    char infoLog[1024];
    if(strcmp(type, "PROGRAM"))
    {
        glad_glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glad_glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n %s\n", type, 
                                                                         infoLog);

        }
    }
    else 
    {
        glad_glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glad_glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            printf("ERROR::SHADER_LINKING_ERROR of type: %s\n %s\n", type, 
                                                                     infoLog);
        }
    }
};

void shader_set_matrix4f(struct shader *item_shader,
                         const char *shader_name,
                         GLfloat *matrix)
{
    glad_glUniformMatrix4fv(glad_glGetUniformLocation(get_id(item_shader),
                                                      shader_name),
                            1,
                            GL_FALSE,
                            (const GLfloat*)matrix);
};

void shader_set_vec3(struct shader *item_shader,
                     const char *shader_name,
                     GLfloat x,
                     GLfloat y,
                     GLfloat z)
{
    glad_glUniform3f(glad_glGetUniformLocation(get_id(item_shader), shader_name),
                     x,
                     y,
                     z);
};

void shader_set_bool(struct shader *item_shader,
                     const char *shader_name,
                     bool value)
{
    glad_glUniform1i(glad_glGetUniformLocation(get_id(item_shader), shader_name),
                     value);
};

void shader_set_float(struct shader *item_shader,
                      const char *shader_name,
                      GLfloat value)
{
    glad_glUniform1f(glad_glGetUniformLocation(get_id(item_shader), shader_name),
                     value);
};
