//
// Created by Matyas Cupr on 07/05/2022.
//

#include "directory_handling.h"


static int sort_dirs(const void *arg1, const void *arg2)
{
    struct directory *dir1 = *(struct directory **)arg1;
    struct directory *dir2 = *(struct directory **)arg2;

    return strcmp(dir1->dir_info->name, dir2->dir_info->name);
}

static int sort_reg_files(const void *arg1, const void *arg2)
{
    struct file_info *file1 = *(struct file_info **)arg1;
    struct file_info *file2 = *(struct file_info **)arg2;

    return strcmp(file1->name, file2->name);
}


static struct directory *get_dir_entries(struct directory *dir)
{
    assert(dir != NULL);

    dir->subdirs = create_array(0, (destructor)free_directory);
    if (dir->subdirs == NULL) {
        fprintf(stderr, "Failed to create subdirectory list\n");
        return NULL;
    }

    dir->reg_files = create_array(0, (destructor)free_file_info);
    if (dir->reg_files == NULL) {
        fprintf(stderr, "Failed to create regular file list\n");
        return NULL;
    }

    string_t dir_path = connect_strings((string_t *)dir->dir_info->path->content, dir->dir_info->path->length, "/");
    if (dir_path == NULL) {
        return NULL;
    }

    DIR *open_dir = opendir(dir_path);
    if (open_dir == NULL) {
        free(dir_path);
        perror("opendir");
        return NULL;
    }

    dir_path = append_char(dir_path, '/');
    size_t dir_path_len = strlen(dir_path);

    struct dirent *entry;
    char entry_path[dir_path_len + 256];
    strcpy(entry_path, dir_path);
    free(dir_path);

    errno = 0;
    while ((entry = readdir(open_dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        strcpy(entry_path + dir_path_len, entry->d_name);

        struct file_info *file_info = get_file_info(entry_path);
        if (file_info == NULL) {
            free(entry_path);
            if (closedir(open_dir) < 0) {
                perror("closedir");
            }
            return NULL;
        }

        switch (file_info->type) {
            case DIRECTORY: {
                struct directory *empty_subdir = malloc(sizeof(struct directory));
                if (empty_subdir == NULL) {
                    if (closedir(open_dir) < 0) {
                        perror("closedir");
                    }
                    return NULL;
                }

                empty_subdir->dir_info = file_info;

                struct directory *filled_subdir = get_dir_entries(empty_subdir);

                dir->subdirs = insert_array_element(dir->subdirs, dir->subdirs->length, filled_subdir);

                break;
            }

            case REGULAR:
                dir->reg_files = insert_array_element(dir->reg_files, dir->reg_files->length, file_info);
                break;

            default:
                fprintf(stderr, "Unknown file type found\n");
                if (closedir(open_dir) < 0) {
                    perror("closedir");
                }
                return NULL;
        }
    }

    if (errno != 0) {
        perror("readdir");
        if (closedir(open_dir) < 0) {
            perror("closedir");
        }
        return NULL;
    }

    if (closedir(open_dir) < 0) {
        perror("closedir");
    }

    qsort(dir->subdirs->content, dir->subdirs->length, sizeof(struct directory *), sort_dirs);
    qsort(dir->reg_files->content, dir->reg_files->length, sizeof(struct file_info *), sort_reg_files);

    return dir;
}

struct directory *get_directory(const char *dir_path)
{
    assert(dir_path != NULL);

    struct directory *empty_dir = malloc(sizeof(struct directory));
    if (empty_dir == NULL) {
        fprintf(stderr, "Failed to allocate directory\n");
        return NULL;
    }

    empty_dir->dir_info = get_file_info(dir_path);
    if (empty_dir->dir_info == NULL) {
        fprintf(stderr, "Failed to fetch directory info\n");
        free_directory(empty_dir);
        return NULL;
    }

    struct directory *filled_dir = get_dir_entries(empty_dir);
    if (filled_dir == NULL) {
        fprintf(stderr, "Failed to fetch directory entries\n");
        free_directory(filled_dir);
        return NULL;
    }

    return filled_dir;
}

void free_directory(struct directory *directory)
{
    if (directory != NULL) {
        free_file_info(directory->dir_info);

        free_array(directory->subdirs);
        free_array(directory->reg_files);
    }

    free(directory);
}
