//
// Created by Matyas Cupr on 05/05/2022.
//

#include "dynamic_string.h"


string_t create_string(size_t length)
{
    string_t str = calloc(length + 1, sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Failed to allocate string\n");
    }

    return str;
}

string_t create_string_from_template(const char *template)
{
    assert(template != NULL);

    string_t str = create_string(strlen(template));
    if (str == NULL) {
        return NULL;
    }

    return strcpy(str, template);
}

string_t reallocate_string(string_t old_str, size_t new_length)
{
    assert(old_str != NULL);

    size_t old_length = strlen(old_str);

    if (new_length == old_length) {
        return old_str;
    }

    string_t new_str = realloc(old_str, (new_length + 1) * sizeof(char));
    if (new_str == NULL) {
        fprintf(stderr, "Failed to reallocate string\n");
        return NULL;
    }

    if (new_length > old_length) {
        for (size_t i = old_length; i <= new_length; i += 1) {
            new_str[i] = '\0';
        }
    }

    return new_str;
}


string_t append_char(string_t str, char c)
{
    assert(str != NULL);

    str = reallocate_string(str, strlen(str) + 1);
    str[strlen(str)] = c;
    return str;
}

string_t append_string(string_t dest_str, const char *src_str)
{
    assert(dest_str != NULL);
    assert(src_str != NULL);

    size_t added_length = strlen(src_str);

    for (size_t i = 0; i < added_length; i += 1) {
        dest_str = append_char(dest_str, src_str[i]);
    }

    return dest_str;
}
