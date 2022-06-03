//
// Created by Matyas Cupr on 05/05/2022.
//

#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


typedef void (*destructor)(void *);

struct linked_list {
    size_t length;

    destructor free_elem;

    struct linked_list_node *head;
    struct linked_list_node *tail;
};

struct linked_list_node {
    void *data;

    struct linked_list_node *next;
    struct linked_list_node *prev;
};


struct linked_list *create_linked_list(destructor free_elem);

struct linked_list *create_linked_list_from_array(void *array[], size_t length, destructor free_elem);

void free_linked_list(struct linked_list *list);


struct linked_list_node *create_linked_list_node(void *data);

void free_linked_list_node(struct linked_list_node *node, void (*data_destructor)(void *));


void prepend_linked_list_node(struct linked_list *list, struct linked_list_node *node);

void append_linked_list_node(struct linked_list *list, struct linked_list_node *node);

struct linked_list_node *get_linked_list_node(struct linked_list *list, size_t index);

void *get_linked_list_elem(struct linked_list *list, size_t index);


#endif //LINKED_LIST_H
