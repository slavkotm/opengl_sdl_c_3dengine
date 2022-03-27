#ifndef MATRIX_INCLUDED
#define MATRIX_INCLUDED
#include <../include/gsl/gsl_blas.h>
#include <../include/gsl/gsl_matrix.h>
#include <math.h>

gsl_matrix *look_at(gsl_vector *,
                    gsl_vector *,
                    gsl_vector *);

gsl_matrix *perspective(double);

#endif
