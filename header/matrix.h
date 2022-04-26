#ifndef MATRIX_INCLUDED
#define MATRIX_INCLUDED
#include "../include/gsl/gsl_blas.h"
#include "../include/gsl/gsl_matrix.h"
#include "../include/gsl/gsl_vector.h"
#include "../header/vector.h"
#include "../include/GL/gl.h"
#include <math.h>

gsl_matrix *matrix_look_at(gsl_vector *eye, gsl_vector *center, gsl_vector *up);
gsl_matrix *matrix_perspective(double FOV, double ASPECT, double ZNEAR, double ZFAR);
gsl_matrix *matrix_model();
void matrix_set_value(gsl_matrix *matrix, int i, int j, double value);
void matrix_destroy(gsl_matrix *matrix);
float *matrix_to_array(gsl_matrix *matrix, float *array, int row, int col);
void matrix_print(gsl_matrix *matrix, int row, int col);

#endif
