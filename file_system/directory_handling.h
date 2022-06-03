//
// Created by Matyas Cupr on 07/05/2022.
//

#ifndef DIRECTORY_HANDLING_H
#define DIRECTORY_HANDLING_H

#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_handling.h"
#include "file_handling.h"


struct directory {
    struct file_info *dir_info;

    array_t *subdirs;
    array_t *reg_files;
};


struct directory *get_directory(const char *path);

void free_directory(struct directory *directory);


#endif //DIRECTORY_HANDLING_H
