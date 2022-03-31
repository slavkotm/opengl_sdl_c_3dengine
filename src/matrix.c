#include "../header/matrix.h"

void matrix_get_float(gsl_matrix *matrix,
                      float **matrixf,
                      int row,
                      int col)
{
    for(int i = 0; i < row; i++)
        for(int j = 0; j < col; j++)
            matrixf[i][j] = gsl_matrix_get(matrix, i, j);
};

gsl_matrix *matrix_look_at(gsl_vector *eye,
                           gsl_vector *center,
                           gsl_vector *up)
{
    gsl_matrix *matrix_result = gsl_matrix_alloc(4, 4);
    gsl_matrix_set_zero(matrix_result);

    gsl_matrix_set(matrix_result, 0, 0, 1);
    gsl_matrix_set(matrix_result, 1, 1, 1);
    gsl_matrix_set(matrix_result, 2, 2, 1);
    gsl_matrix_set(matrix_result, 3, 3, 1);

    gsl_vector *f = NULL;
    gsl_vector *s = NULL; 
    gsl_vector *u = NULL;

    f = vector_normalize(vector_subtraction((const gsl_vector *)center, (const gsl_vector *)eye, 3), 3); 
    s = vector_normalize(vector_cross(f, up), 3);
    u = vector_cross(s, f);

    gsl_matrix_set(matrix_result, 0, 0, gsl_vector_get(s, 0));
    gsl_matrix_set(matrix_result, 1, 0, gsl_vector_get(s, 1));
    gsl_matrix_set(matrix_result, 2, 0, gsl_vector_get(s, 2));

    gsl_matrix_set(matrix_result, 0, 1, gsl_vector_get(u, 0));
    gsl_matrix_set(matrix_result, 1, 1, gsl_vector_get(u, 1));
    gsl_matrix_set(matrix_result, 2, 1, gsl_vector_get(u, 2));

    gsl_matrix_set(matrix_result, 0, 2, -gsl_vector_get(f, 0));
    gsl_matrix_set(matrix_result, 1, 2, -gsl_vector_get(f, 1));
    gsl_matrix_set(matrix_result, 2, 2, -gsl_vector_get(f, 2));

    gsl_matrix_set(matrix_result, 3, 0, -vector_dot(s, eye, 3));
    gsl_matrix_set(matrix_result, 3, 1, -vector_dot(u, eye, 3));
    gsl_matrix_set(matrix_result, 3, 2,  vector_dot(f, eye, 3));

    return matrix_result;
};

gsl_matrix *matrix_perspective(double FOV,
                               double ASPECT,
                               double ZNEAR,
                               double ZFAR)
{
    gsl_matrix *matrix_result = gsl_matrix_alloc(4, 4);
    gsl_matrix_set_zero(matrix_result);
    double tan_half_fov = tan(FOV / (double)2);

    gsl_matrix_set(matrix_result, 0, 0, (double)1 / (ASPECT * tan_half_fov));
    gsl_matrix_set(matrix_result, 1, 1, (double)1 / (tan_half_fov));
    gsl_matrix_set(matrix_result, 2, 2, - (ZFAR + ZNEAR) / (ZFAR - ZNEAR));
    gsl_matrix_set(matrix_result, 2, 3, - (double)1);
    gsl_matrix_set(matrix_result, 3, 2, - ((double)2 * ZFAR * ZNEAR) / (ZFAR - ZNEAR));
    
    return matrix_result;
};

void matrix_destroy(gsl_matrix *matrix) { gsl_matrix_free(matrix); };

void matrix_print(gsl_matrix *matrix,
                  int row,
                  int col)
{
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            printf("%.3lf ", gsl_matrix_get(matrix, i, j));
        }
        printf("\n");
    }
};
