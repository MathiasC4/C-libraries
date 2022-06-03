//
// Created by Matyas Cupr on 05/05/2022.
//

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H


#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef char * string_t;


string_t create_string(size_t length);

string_t create_string_from_template(const char *template);

string_t reallocate_string(string_t old_str, size_t new_length);


string_t append_char(string_t str, char c);

string_t append_string(string_t dest_str, const char *src_str);


#endif //DYNAMIC_STRING_H
