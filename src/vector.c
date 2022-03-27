#include "../header/vector.h"

gsl_vector *vector_normalize(const gsl_vector *vector,
                             int dimension_vector)
{
    gsl_vector *vector_normalize = NULL;

    if(dimension_vector > 0)
    {
        vector_normalize = gsl_vector_alloc(dimension_vector);
        double vector_length;

        for(int i = 0; i < dimension_vector; i++)
            vector_length += pow(gsl_vector_get(vector, i), 2);

        vector_length = sqrt(vector_length);

        for(int i = 0; i < dimension_vector; i++)
            gsl_vector_set(vector_normalize, i, (gsl_vector_get(vector, i) / vector_length));
    }

    return vector_normalize;
};

gsl_vector *vector_cross(const gsl_vector *vector_first,
                         const gsl_vector *vector_second)
{
    gsl_vector *vector_cross = gsl_vector_alloc(3);

    gsl_vector_set(vector_cross, 0, gsl_vector_get(vector_first, 1) * gsl_vector_get(vector_second, 2) - 
                                    gsl_vector_get(vector_first, 2) * gsl_vector_get(vector_second, 1));

    gsl_vector_set(vector_cross, 1, gsl_vector_get(vector_first, 2) * gsl_vector_get(vector_second, 0) - 
                                    gsl_vector_get(vector_first, 0) * gsl_vector_get(vector_second, 2));

    gsl_vector_set(vector_cross, 2, gsl_vector_get(vector_first, 0) * gsl_vector_get(vector_second, 1) - 
                                    gsl_vector_get(vector_first, 1) * gsl_vector_get(vector_second, 0));

    return vector_cross;
};

double vector_dot(const gsl_vector *vector_first,
                  const gsl_vector *vector_second,
                  int dimension_vectors)
{
    double dot_result = 0;

    for(int i = 0; i < dimension_vectors; i++)
        dot_result += (gsl_vector_get(vector_first, i) * gsl_vector_get(vector_second, i));

    return dot_result;
};

void vector_destroy(gsl_vector *vector) { gsl_vector_free(vector); };
