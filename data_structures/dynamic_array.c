//
// Created by Matyas Cupr on 06/05/2022.
//

#include "dynamic_array.h"


array_t *create_array(size_t length, destructor free_elem)
{
    array_t *array = calloc(1, sizeof(struct array) + length * sizeof(void *));
    if (array == NULL) {
        fprintf(stderr, "Failed to allocate array\n");
    }

    array->length = length;
    array->free_elem = free_elem;

    return array;
}

array_t *reallocate_array(array_t *old_array, size_t new_length)
{
    assert(old_array != NULL);

    destructor free_elem = old_array->free_elem;
    size_t old_length = old_array->length;

    if (new_length == old_length) {
        return old_array;
    }

    if (new_length < old_length && free_elem != NULL) {
        for (size_t i = new_length; i < old_length ; i += 1) {
            free_elem(old_array->content[i]);
        }
    }

    array_t *new_array = realloc(old_array, sizeof(struct array) + new_length * sizeof(void *));
    if (new_array == NULL) {
        fprintf(stderr, "Failed to reallocate array\n");
        return NULL;
    }

    new_array->length = new_length;
    new_array->free_elem = free_elem;

    if (new_length > old_length) {
        for (size_t i = old_length; i < new_length; i += 1) {
            new_array->content[i] = NULL;
        }
    }

    return new_array;
}

void free_array(array_t *array)
{
    if (array != NULL && array->free_elem != NULL) {
        for (size_t i = 0; i < array->length; i += 1) {
            array->free_elem(array->content[i]);
        }
    }

    free(array);
}


array_t *insert_array_element(array_t *array, size_t index, void *data)
{
    assert(array != NULL);

    if (index >= array->length + 1) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }

    array = reallocate_array(array, array->length + 1);
    if (array == NULL) {
        return NULL;
    }

    for (size_t i = array->length - 1; i > index; i -= 1) {
        array->content[i] = array->content[i - 1];
    }

    array->content[index] = data;

    return array;
}

array_t *prepend_array_element(array_t *array, void *data)
{
    assert(array != NULL);

    return insert_array_element(array, 0, data);
}

array_t *append_array_element(array_t *array, void *data)
{
    assert(array != NULL);

    return insert_array_element(array, array->length, data);
}

array_t *remove_array_element(array_t *array, size_t index)
{
    assert(array != NULL);

    if (index >= array->length) {
        fprintf(stderr, "Index out of bounds\n");
        return NULL;
    }

    if (array->free_elem != NULL) {
        array->free_elem(array->content[index]);
    }

    for (size_t i = index; i < array->length - 1; i += 1) {
        array->content[i] = array->content[i + 1];
    }

    array = reallocate_array(array, array->length - 1);
    if (array == NULL) {
        return NULL;
    }

    return array;
}


size_t find_in_array(array_t *array, void *target, comparator compare_elems)
{
    assert(array != NULL);
    assert(target != NULL);
    assert(compare_elems != NULL);

    for (size_t index = 0; index < array->length; index += 1) {
        if (compare_elems(array->content[index], target) == 0) {
            return index;
        }
    }

    return array->length;
}

void sort_array(array_t *array, comparator compare_elems)
{
    qsort(array->content, array->length, sizeof(void *), compare_elems);
}
