#include "../include/glad/glad.h"
#include "../include/SDL2/SDL.h" 
#include "../include/GL/gl.h"
#include "../include/gsl/gsl_blas.h"
#include "../include/gsl/gsl_matrix_double.h"
#include <math.h>
#include <stdbool.h>
#include "../header/read.h"
#include "../header/shader.h"
#include "../header/init.h"
#include "../header/window.h"
#include "../header/context.h"
#include "../header/event.h"
#define PI 3.141592654

enum { EPS = 0, LEFT, RIGHT, START, END, BEHIND, BEYOND, BEETWEN };

int classify(const gsl_vector *a, const gsl_vector *b, const gsl_vector *p)
{
    const gsl_vector *a2b = gsl_vector_alloc(2);
    gsl_vector_set((gsl_vector *)a2b, 0, gsl_vector_get(b, 0) - gsl_vector_get(a, 0));
    gsl_vector_set((gsl_vector *)a2b, 1, gsl_vector_get(b, 1) - gsl_vector_get(a, 1));

    const gsl_vector *a2p = gsl_vector_alloc(2);
    gsl_vector_set((gsl_vector *)a2p, 0, gsl_vector_get(p, 0) - gsl_vector_get(a, 0));
    gsl_vector_set((gsl_vector *)a2p, 1, gsl_vector_get(p, 1) - gsl_vector_get(a, 1));

    double area = gsl_vector_get(a2b, 0) * gsl_vector_get(a2p, 1) - gsl_vector_get(a2p, 0) * gsl_vector_get(a2b, 1); 

    if(area > (double)EPS)
    {
        printf("LEFT\n");
        return LEFT;
    }

    if(area < (double)(-1) * (double)EPS)
    {
        printf("RIGHT\n");
        return RIGHT;
    }

    if(sqrt(pow(gsl_vector_get(p, 0) - gsl_vector_get(a, 0), 2) + pow(gsl_vector_get(p, 1) - gsl_vector_get(a, 1), 2)) < (double)EPS)
    {
        printf("START\n");
        return START;
    }

    if(sqrt(pow(gsl_vector_get(p, 0) - gsl_vector_get(b, 0), 2) + pow(gsl_vector_get(p, 1) - gsl_vector_get(b, 1), 2)) < (double)EPS)
    {
        printf("END\n");
        return END;
    }

    if((gsl_vector_get(a2b, 0) * gsl_vector_get(a2p, 0) < (double)0) || (gsl_vector_get(a2b, 1) * gsl_vector_get(a2p, 1) < (double)0))
    {
        printf("BEHIND\n");
        return BEHIND;
    }

    if(sqrt(pow(gsl_vector_get(a2b, 0), 2) + pow(gsl_vector_get(a2b, 1), 2)) < sqrt(pow(gsl_vector_get(a2p, 0), 2) + pow(gsl_vector_get(a2p, 1), 2)))
    {
        printf("BEYOND\n");
        return BEYOND;
    }

    printf("BEETWEN\n");
    return BEETWEN;
}

gsl_vector *normalize(const gsl_vector *vector, int dimension_vector)
{
    gsl_vector *vector_normalize;
    if(dimension_vector == 2)
    {
    }
    if(dimension_vector == 3)
    {
        vector_normalize = gsl_vector_alloc(dimension_vector);
        double length = sqrt(pow(gsl_vector_get(vector, 0), 2) + pow(gsl_vector_get(vector, 1), 2) + pow(gsl_vector_get(vector, 2), 2));
        gsl_vector_set((gsl_vector *)vector_normalize, 0, gsl_vector_get(vector, 0) / length);
        gsl_vector_set((gsl_vector *)vector_normalize, 1, gsl_vector_get(vector, 1) / length);
        gsl_vector_set((gsl_vector *)vector_normalize, 2, gsl_vector_get(vector, 2) / length);
    }
    return vector_normalize;
}

gsl_vector *cross(const gsl_vector *vector_one, const gsl_vector *vector_two)
{
    gsl_vector *vector = gsl_vector_alloc(3);

    gsl_vector_set(vector, 0, gsl_vector_get(vector_one, 1) * gsl_vector_get(vector_two, 2) - gsl_vector_get(vector_one, 2) * gsl_vector_get(vector_two, 1));
    gsl_vector_set(vector, 1, gsl_vector_get(vector_one, 2) * gsl_vector_get(vector_two, 0) - gsl_vector_get(vector_one, 0) * gsl_vector_get(vector_two, 2));
    gsl_vector_set(vector, 2, gsl_vector_get(vector_one, 0) * gsl_vector_get(vector_two, 1) - gsl_vector_get(vector_one, 1) * gsl_vector_get(vector_two, 0));

    return vector;
}

double dot(gsl_vector *vector_one, gsl_vector *vector_two)
{
    return gsl_vector_get(vector_one, 0) * gsl_vector_get(vector_two, 0) + 
           gsl_vector_get(vector_one, 1) * gsl_vector_get(vector_two, 1) + 
           gsl_vector_get(vector_one, 2) * gsl_vector_get(vector_two, 2);
}

gsl_matrix *look_at(gsl_vector *eye, gsl_vector *center, gsl_vector *up)
{
    gsl_matrix *matrix_result = gsl_matrix_alloc(4, 4);
    gsl_vector *f = gsl_vector_alloc(3);
    gsl_vector *s = gsl_vector_alloc(3);
    gsl_vector *u = gsl_vector_alloc(3);

    gsl_vector *vector = gsl_vector_alloc(3);
    gsl_vector_set(vector, 0, gsl_vector_get(center, 0) - gsl_vector_get(eye, 0));
    gsl_vector_set(vector, 1, gsl_vector_get(center, 1) - gsl_vector_get(eye, 1));
    gsl_vector_set(vector, 2, gsl_vector_get(center, 2) - gsl_vector_get(eye, 2));

    f = normalize(vector, 3);

    s = normalize(cross(f, up), 3);

    u = cross(s, f);

    gsl_matrix_set(matrix_result, 0, 0, gsl_vector_get(s, 0));
    gsl_matrix_set(matrix_result, 1, 0, gsl_vector_get(s, 1));
    gsl_matrix_set(matrix_result, 2, 0, gsl_vector_get(s, 2));

    gsl_matrix_set(matrix_result, 0, 1, gsl_vector_get(u, 0));
    gsl_matrix_set(matrix_result, 1, 1, gsl_vector_get(u, 1));
    gsl_matrix_set(matrix_result, 2, 1, gsl_vector_get(u, 2));

    gsl_matrix_set(matrix_result, 0, 2, -gsl_vector_get(f, 0));
    gsl_matrix_set(matrix_result, 1, 2, -gsl_vector_get(f, 1));
    gsl_matrix_set(matrix_result, 2, 2, -gsl_vector_get(f, 2));

    gsl_matrix_set(matrix_result, 3, 0, -dot(s, eye));
    gsl_matrix_set(matrix_result, 3, 1, -dot(u, eye));
    gsl_matrix_set(matrix_result, 3, 2,  dot(f, eye));
    gsl_matrix_set(matrix_result, 3, 3,  1.0f);

    return matrix_result;
}

bool firstMouse = true;
float pitch =  0.0f;
float lastX =  1200.0f / 2.0;
float lastY =  800.0f / 2.0;
float fov   =  120.0f;
float yaw   = -90.0f;

gsl_vector *mouse_callback(double xposIn, double yposIn)
{
    float xpos = (float)(xposIn);
    float ypos = (float)(yposIn);

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    gsl_vector *direction = gsl_vector_alloc(3);
    gsl_vector_set(direction, 0, cos(yaw) * cos(pitch));
    gsl_vector_set(direction, 1, sin(pitch));
    gsl_vector_set(direction, 2, sin(yaw) * cos(pitch));

    return normalize(direction, 3);
}

int main(int argc, 
         char **argv)
{
    init(true, false);

    SDL_Window *window = window_create("OpenGL + C + SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_OPENGL);
    
    SDL_GLContext gl_context = context_create(window);
    context_set(3, 3);

    /*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);*/

    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

    SDL_ShowCursor(SDL_DISABLE);

    SDL_Event event;
    int running = 1;

    struct shader *shaders = shader_malloc();

    shader_init(shaders, "shaders/basic.vert", "shaders/basic.frag", "r");

    gsl_vector *va = gsl_vector_alloc(3);
    gsl_vector *vb = gsl_vector_alloc(3);
    gsl_vector *p = gsl_vector_alloc(3);

    gsl_vector_set(va, 0, 0.0f);
    gsl_vector_set(va, 1, 0.0f);
    gsl_vector_set(va, 2, 0.0f);

    gsl_vector_set(vb, 0, -0.5f);
    gsl_vector_set(vb, 1, 0.0f);
    gsl_vector_set(vb, 2, 0.0f);

    gsl_vector_set(p, 0, 0.1f);
    gsl_vector_set(p, 1, 0.5f);
    gsl_vector_set(p, 2, 0.0f);

    gsl_matrix *model = gsl_matrix_alloc(4, 4);
    gsl_matrix_set_zero(model);
    gsl_matrix_set(model, 0, 0, 1.0f);
    gsl_matrix_set(model, 1, 1, 0.573576f);
    gsl_matrix_set(model, 1, 2, -0.819152f);
    gsl_matrix_set(model, 2, 2, 0.573576f);
    gsl_matrix_set(model, 2, 1, 0.819152f);
    gsl_matrix_set(model, 3, 3, 1.0f);

    gsl_matrix *view = gsl_matrix_alloc(4, 4);
    gsl_matrix_set_zero(view);
    gsl_matrix_set(view, 0, 0, 1.0f);
    gsl_matrix_set(view, 1, 1, 1.0f);
    gsl_matrix_set(view, 2, 2, 1.0f);
    gsl_matrix_set(view, 3, 2, -6.0f);
    gsl_matrix_set(view, 3, 3, 1.0f);

    gsl_matrix *projection = gsl_matrix_alloc(4, 4);
    gsl_matrix_set_zero(projection);
    gsl_matrix_set(projection, 0, 0, (double)1 / (double)((1.5) * tan(PI / 8)));
    gsl_matrix_set(projection, 1, 1, (double)1 / (tan(PI / 8)));
    gsl_matrix_set(projection, 2, 2, -(100.0f + 0.1f) / (100.0f - 0.1f));
    gsl_matrix_set(projection, 2, 3, -1.0f);
    gsl_matrix_set(projection, 3, 2, -(2.0f * 100.0f * 0.1f) / (100.0f - 0.1f));

    GLfloat matrix_model[4][4];
    GLfloat matrix_view[4][4];
    GLfloat matrix_projection[4][4];

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            matrix_view[i][j] = (GLfloat)gsl_matrix_get(view, i, j);
            printf("%.3lf ", matrix_view[i][j]);
        }
        printf("\n");
    }
        printf("\n");
    
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            matrix_projection[i][j] = (GLfloat)gsl_matrix_get(projection, i, j);
            printf("%.3lf ", matrix_projection[i][j]);
        }
        printf("\n");
    }
        printf("\n");

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            matrix_model[i][j] = (GLfloat)gsl_matrix_get(model, i, j);
            printf("%.3lf ", matrix_model[i][j]);
        }
        printf("\n");
    }

    GLfloat line[] = {
        (GLfloat)gsl_vector_get(va, 0), (GLfloat)gsl_vector_get(va, 1), (GLfloat)gsl_vector_get(va, 2),
        (GLfloat)gsl_vector_get(vb, 0), (GLfloat)gsl_vector_get(vb, 1), (GLfloat)gsl_vector_get(vb, 2)
    };

    GLfloat point[] = {
        (GLfloat)gsl_vector_get(p, 0), (GLfloat)gsl_vector_get(p, 1), (GLfloat)gsl_vector_get(p, 2)
    };

    GLfloat vertex[] = {
       -0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,    0.5f, 0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
        0, 4, 1,
        1, 4, 5,
        0, 3, 7,
        0, 7, 4,
        1, 6, 2,
        1, 5, 6,
        2, 7, 3,
        2, 6, 7,
        4, 7, 5,
        5, 7, 6
    };

    GLuint VBOs[2], VAOs[2];
    glad_glGenVertexArrays(2, VAOs);
    glad_glGenBuffers(2, VBOs);

    glad_glBindVertexArray(VAOs[0]);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glad_glEnableVertexAttribArray(0);

    glad_glBindVertexArray(VAOs[1]);
    glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glad_glEnableVertexAttribArray(0);

    GLuint VBO, VAO, EBO;
    glad_glGenVertexArrays(1, &VAO);
    glad_glGenBuffers(1, &VBO);
    glad_glGenBuffers(1, &EBO);

    glad_glBindVertexArray(VAO);

    glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)0);
    glad_glEnableVertexAttribArray(0);
    glad_glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glad_glEnableVertexAttribArray(1);

    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);

    glad_glBindVertexArray(0);


    glad_glEnable(GL_DEPTH_TEST);  
    /*glad_glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
    GLfloat r = 0.2f, g = 0.3f, b = 0.3f;
    double angle = 0.0;
    bool pause = true;

    gsl_vector *cameraPos = gsl_vector_alloc(3);
    gsl_vector_set(cameraPos, 0, 0.0f);
    gsl_vector_set(cameraPos, 1, 0.0f);
    gsl_vector_set(cameraPos, 2, 3.0f);

    gsl_vector *cameraFront = gsl_vector_alloc(3);
    gsl_vector_set(cameraFront, 0, 0.0f);
    gsl_vector_set(cameraFront, 1, 0.0f);
    gsl_vector_set(cameraFront, 2, -1.0f);

    gsl_vector *cameraUp = gsl_vector_alloc(3);
    gsl_vector_set(cameraUp, 0, 0.0f);
    gsl_vector_set(cameraUp, 1, 1.0f);
    gsl_vector_set(cameraUp, 2, 0.0f);

    gsl_vector *cameraPosFront = gsl_vector_alloc(3);
    gsl_vector_set(cameraPosFront, 0, gsl_vector_get(cameraPos, 0) + gsl_vector_get(cameraFront, 0));
    gsl_vector_set(cameraPosFront, 1, gsl_vector_get(cameraPos, 1) + gsl_vector_get(cameraFront, 1));
    gsl_vector_set(cameraPosFront, 2, gsl_vector_get(cameraPos, 2) + gsl_vector_get(cameraFront, 2));

        printf("\n");
    gsl_matrix *view_new = gsl_matrix_alloc(4, 4);
    gsl_matrix_set_zero(view_new);
    gsl_matrix_set(view_new, 0, 0, 1.0f);
    gsl_matrix_set(view_new, 1, 1, 1.0f);
    gsl_matrix_set(view_new, 2, 2, 1.0f);
    gsl_matrix_set(view_new, 3, 3, 1.0f);
    view_new = look_at(cameraPos, cameraPosFront, cameraUp);

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%.3lf ", gsl_matrix_get(view_new, i, j));
        }
        printf("\n");
    }

    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            matrix_view[i][j] = gsl_matrix_get(view_new, i, j);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%.3lf ", matrix_view[i][j]);
        }
        printf("\n");
    }
    matrix_view[3][3] = 1.0f;
    const double cameraSpeed = 0.05;


    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_WarpMouseInWindow(window, 600, 400);

    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = 0;
            }
            if(event.type == SDL_MOUSEMOTION)
            {
                cameraFront = mouse_callback(event.motion.x, event.motion.y); 
                printf("%.3lf\n", gsl_vector_get(cameraFront, 0));
                printf("%.3lf\n", gsl_vector_get(cameraFront, 1));
                printf("%.3lf\n", gsl_vector_get(cameraFront, 2));
            }
            if(event.type == SDL_KEYDOWN) 
            {
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
                if(event.key.keysym.sym == SDLK_1)
                {
                    g = b = 0.0f;
                    r = 1.0f;
                }
                if(event.key.keysym.sym == SDLK_2)
                {
                    r = b = 0.0f;
                    g = 1.0f;
                }
                if(event.key.keysym.sym == SDLK_3)
                {
                    g = r = 0.0f;
                    b = 1.0f;
                }
                if(event.key.keysym.sym == SDLK_4)
                {
                    b = g = 0.3f;
                    r = 0.2f;
                }
                if(event.key.keysym.sym == SDLK_p)
                {
                    pause = false;
                }
                if(event.key.keysym.sym == SDLK_x)
                {
                    pause = true;
                }
                if(event.key.keysym.sym == SDLK_w)
                {
                    gsl_vector_set(cameraPos, 0, gsl_vector_get(cameraPos, 0) + cameraSpeed * gsl_vector_get(cameraFront, 0));
                    gsl_vector_set(cameraPos, 1, gsl_vector_get(cameraPos, 1) + cameraSpeed * gsl_vector_get(cameraFront, 1));
                    gsl_vector_set(cameraPos, 2, gsl_vector_get(cameraPos, 2) + cameraSpeed * gsl_vector_get(cameraFront, 2));
                }
                if(event.key.keysym.sym == SDLK_a)
                {
                    gsl_vector *vec = normalize(cross(cameraFront, cameraUp), 3);
                    gsl_vector_set(cameraPos, 0, gsl_vector_get(cameraPos, 0) - cameraSpeed * gsl_vector_get(vec, 0));
                    gsl_vector_set(cameraPos, 1, gsl_vector_get(cameraPos, 1) - cameraSpeed * gsl_vector_get(vec, 1));
                    gsl_vector_set(cameraPos, 2, gsl_vector_get(cameraPos, 2) - cameraSpeed * gsl_vector_get(vec, 2));
                }
                if(event.key.keysym.sym == SDLK_s)
                {
                    gsl_vector_set(cameraPos, 0, gsl_vector_get(cameraPos, 0) - cameraSpeed * gsl_vector_get(cameraFront, 0));
                    gsl_vector_set(cameraPos, 1, gsl_vector_get(cameraPos, 1) - cameraSpeed * gsl_vector_get(cameraFront, 1));
                    gsl_vector_set(cameraPos, 2, gsl_vector_get(cameraPos, 2) - cameraSpeed * gsl_vector_get(cameraFront, 2));
                }
                if(event.key.keysym.sym == SDLK_d)
                {
                    gsl_vector *vec = normalize(cross(cameraFront, cameraUp), 3);
                    gsl_vector_set(cameraPos, 0, gsl_vector_get(cameraPos, 0) + cameraSpeed * gsl_vector_get(vec, 0));
                    gsl_vector_set(cameraPos, 1, gsl_vector_get(cameraPos, 1) + cameraSpeed * gsl_vector_get(vec, 1));
                    gsl_vector_set(cameraPos, 2, gsl_vector_get(cameraPos, 2) + cameraSpeed * gsl_vector_get(vec, 2));
                }
            }
        }

        glad_glClearColor(r, g, b, 1.0f);
        glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gsl_vector_set(cameraPosFront, 0, gsl_vector_get(cameraPos, 0) + gsl_vector_get(cameraFront, 0));
        gsl_vector_set(cameraPosFront, 1, gsl_vector_get(cameraPos, 1) + gsl_vector_get(cameraFront, 1));
        gsl_vector_set(cameraPosFront, 2, gsl_vector_get(cameraPos, 2) + gsl_vector_get(cameraFront, 2));
        view_new = look_at(cameraPos, cameraPosFront, cameraUp);
        matrix_view[0][0] = gsl_matrix_get(view_new, 0, 0);
        matrix_view[0][1] = gsl_matrix_get(view_new, 0, 1);
        matrix_view[0][2] = gsl_matrix_get(view_new, 0, 2);
        matrix_view[0][3] = gsl_matrix_get(view_new, 0, 3);

        matrix_view[1][0] = gsl_matrix_get(view_new, 1, 0);
        matrix_view[1][1] = gsl_matrix_get(view_new, 1, 1);
        matrix_view[1][2] = gsl_matrix_get(view_new, 1, 2);
        matrix_view[1][3] = gsl_matrix_get(view_new, 1, 3);

        matrix_view[2][0] = gsl_matrix_get(view_new, 2, 0);
        matrix_view[2][1] = gsl_matrix_get(view_new, 2, 1);
        matrix_view[2][2] = gsl_matrix_get(view_new, 2, 2);
        matrix_view[2][3] = gsl_matrix_get(view_new, 2, 3);

        matrix_view[3][0] = gsl_matrix_get(view_new, 3, 0);
        matrix_view[3][1] = gsl_matrix_get(view_new, 3, 1);
        matrix_view[3][2] = gsl_matrix_get(view_new, 3, 2);
        matrix_view[3][3] = gsl_matrix_get(view_new, 3, 3);

        shader_use(shaders);
        GLuint transformLoc = glad_glGetUniformLocation(get_id(shaders), "model");
        glad_glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const GLfloat *)matrix_model);
        GLuint view = glad_glGetUniformLocation(get_id(shaders), "view");
        glad_glUniformMatrix4fv(view, 1, GL_FALSE, (const GLfloat *)matrix_view);
        GLuint projection = glad_glGetUniformLocation(get_id(shaders), "projection");
        glad_glUniformMatrix4fv(projection, 1, GL_FALSE, (const GLfloat *)matrix_projection);

        matrix_model[1][1] = cos(PI * angle / 180);
        matrix_model[2][1] = sin(PI * angle / 180);
        matrix_model[1][2] = -sin(PI * angle / 180);
        matrix_model[2][2] = cos(PI * angle / 180);
        if(pause)
            angle += 1.0;

        glad_glBindVertexArray(VAO);
        glad_glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        /*glad_glBindVertexArray(VAOs[0]);
        glad_glDrawArrays(GL_LINES, 0, 2);

        glad_glBindVertexArray(VAOs[1]);
        glad_glDrawArrays(GL_POINTS, 0, 1);

        printf("%d\n", classify(va, vb, p));*/

        SDL_GL_SwapWindow(window);
    }

    printf("%s\n", glGetString(GL_VERSION));

    free(shaders);

    glad_glDeleteVertexArrays(1, &VAO);
    glad_glDeleteBuffers(1, &VBO); 
    glad_glDeleteBuffers(1, &EBO); 

    glad_glDeleteVertexArrays(2, VAOs);
    glad_glDeleteBuffers(2, VBOs); 

    context_delete(&gl_context);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
