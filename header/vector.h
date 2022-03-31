#ifndef VECTOR_INCLUDED
#define VECTOR_INCLUDED
#include "../include/gsl/gsl_blas.h"
#include "../include/gsl/gsl_vector.h"
#include <math.h>

gsl_vector *vector_normalize(const gsl_vector *,
                             int );

gsl_vector *vector_cross(const gsl_vector *,
                         const gsl_vector *);

double vector_dot(const gsl_vector *,
                  const gsl_vector *,
                  int );

gsl_vector *vector_subtraction(const gsl_vector *,
                               const gsl_vector *,
                               int );

gsl_vector *vector_sum(const gsl_vector *,
                       const gsl_vector *,
                       int );

void vector_print(gsl_vector *,
                  int );

void vector_destroy(gsl_vector *);

#endif
