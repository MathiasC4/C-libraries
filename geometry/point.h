//
// Created by Matyas Cupr on 17/05/2022.
//

#ifndef POINT_H
#define POINT_H


#include <stdarg.h>

#include "data_structures/matrix.h"


typedef struct matrix point;

point *create_point(size_t dimension, ...);

size_t get_point_dimension(point *point);

double get_point_coordinate(point *point, size_t index);

double find_point_distance(point *point1, point *point2);


#endif //POINT_H
