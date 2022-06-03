//
// Created by Matyas Cupr on 05/05/2022.
//

#include "linked_list.h"


struct linked_list *create_linked_list(destructor free_elem)
{
    struct linked_list *list = malloc(sizeof(struct linked_list));
    if (list == NULL) {
        fprintf(stderr, "Failed to allocate list\n");
        return NULL;
    }

    list->length = 0;
    list->free_elem = free_elem;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

struct linked_list *create_linked_list_from_array(void *array[], size_t length, destructor free_elem)
{
    assert(array != NULL);

    struct linked_list *list = create_linked_list(free_elem);
    if (list == NULL) {
        fprintf(stderr, "Failed to create list\n");
        return NULL;
    }

    for (size_t i = 0; i < length; i += 1) {
        struct linked_list_node *node = create_linked_list_node(array[i]);
        if (node == NULL) {
            fprintf(stderr, "Failed to create node\n");
            free_linked_list(list);
            return NULL;
        }

        append_linked_list_node(list, node);
    }

    return list;
}

void free_linked_list(struct linked_list *list)
{
    if (list != NULL) {
        for (size_t i = 0; i < list->length; i += 1) {
            struct linked_list_node *new_head = list->head->next;
            free_linked_list_node(list->head, list->free_elem);
            list->head = new_head;
        }
    }

    free(list);
}


struct linked_list_node *create_linked_list_node(void *data)
{
    struct linked_list_node *node = malloc(sizeof(struct linked_list_node));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate node\n");
        return NULL;
    }

    node->data = data;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void free_linked_list_node(struct linked_list_node *node, destructor free_elem)
{
    if (node != NULL) {
        if (free_elem != NULL) {
            free_elem(node->data);
        }
    }

    free(node);
}


void prepend_linked_list_node(struct linked_list *list, struct linked_list_node *node)
{
    assert(list != NULL);
    assert(node != NULL);

    if (list->length == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->head->prev = node;
        list->head->prev->next = list->head;
        list->head = list->head->prev;
    }

    list->length += 1;
}

void append_linked_list_node(struct linked_list *list, struct linked_list_node *node)
{
    assert(list != NULL);
    assert(node != NULL);

    if (list->length == 0) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail->next->prev = list->tail;
        list->tail = list->tail->next;
    }

    list->length += 1;
}

struct linked_list_node *get_linked_list_node(struct linked_list *list, size_t index)
{
    assert(list != NULL);

    if (index >= list->length) {
        fprintf(stderr, "Invalid node index\n");
        return NULL;
    }

    size_t middle = (list->length - 1) / 2;

    struct linked_list_node *node;

    if (index <= middle) {
        node = list->head;

        for (size_t i = 0; i < index; i += 1) {
            node = node->next;
        }
    }

    if (index > middle) {
        node = list->tail;

        for (size_t i = list->length - 1; i > index; i -= 1) {
            node = node->prev;
        }
    }

    return node;
}

void *get_linked_list_elem(struct linked_list *list, size_t index)
{
    struct linked_list_node *node = get_linked_list_node(list, index);

    return (node == NULL ? NULL : node->data);
}
