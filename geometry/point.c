//
// Created by Matyas Cupr on 17/05/2022.
//

#include "point.h"


point *create_point(size_t dimension, ...)
{
    if (dimension == 0) {
        return NULL;
    }

    struct matrix *point = create_matrix(dimension, 1);
    if (point == NULL) {
        return NULL;
    }

    va_list coords;
    va_start(coords, dimension);

    for (size_t i = 0; i < dimension; i += 1) {
        set_matrix_element(point, i, 0 , va_arg(coords, double));
    }

    va_end(coords);

    return point;
}

size_t get_point_dimension(point *point)
{
    assert(point != NULL);

    return point->rows;
}

double get_point_coordinate(point *point, size_t index)
{
    assert(point != NULL);

    return get_matrix_element(point, index, 0);
}

double find_point_distance(point *point1, point *point2)
{
    assert(point1 != NULL);
    assert(point2 != NULL);

    if (get_point_dimension(point1) != get_point_dimension(point2)) {
        fprintf(stderr, "Incompatible point dimensions\n");
        return -1;
    }

    double sum_of_squares = 0;

    for (size_t i = 0; i < get_point_dimension(point1); i += 1) {
        sum_of_squares += pow(get_point_coordinate(point1, i) - get_point_coordinate(point2, i), 2);
    }

    return sqrt(sum_of_squares);
}
