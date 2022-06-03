//
// Created by Matyas Cupr on 06/05/2022.
//

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H


#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


typedef void (*destructor)(void *);

struct array {
    size_t length;

    destructor free_elem;

    void *content[];
};

typedef struct array array_t;


array_t *create_array(size_t length, destructor free_elem);

array_t *reallocate_array(array_t *old_array, size_t new_length);

void free_array(array_t *array);


array_t *insert_array_element(array_t *array, size_t index, void *data);

array_t *prepend_array_element(array_t *array, void *data);

array_t *append_array_element(array_t *array, void *data);

array_t *remove_array_element(array_t *array, size_t index);


typedef int (*comparator)(const void *, const void *);


size_t find_in_array(array_t *array, void *target, comparator compare_elems);

void sort_array(array_t *array, comparator compare_elems);


#endif //DYNAMIC_ARRAY_H
