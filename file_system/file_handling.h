//
// Created by Matyas Cupr on 05/05/2022.
//

#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "data_structures/dynamic_array.h"
#include "data_structures/dynamic_string.h"
#include "file_system/user_handling.h"
#include "string_handling.h"


enum file_type {
    UNKNOWN,
    REGULAR,
    DIRECTORY,
    CHAR_DEVICE,
    BLOCK_DEVICE,
    FIFO,
    SYM_LINK,
    SOCKET
};

struct file_info {
    array_t *path;
    string_t name;

    enum file_type type;

    char *owner;
    char *group;
    char permissions[9 + 1];
    char flags[3 + 1];
};


struct file_info *get_file_info(const char *file_path);

void free_file_info(struct file_info *file_info);


string_t fget_line(FILE *file);


#endif //FILE_HANDLING_H
