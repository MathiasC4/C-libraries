//
// Created by Matyas Cupr on 05/05/2022.
//

#include "file_handling.h"


static char *get_path(char *path)
{
    assert(path != NULL);

    if (strlen(path) <= 0) {
        fprintf(stderr, "Invalid path\n");
        return NULL;
    }

    char *name = path;
    char *search;

    while ((search = strchr(name, '/')) != NULL && *(search + 1) != '\0') {
        name = search + 1;
    }

    return name;
}

static enum file_type get_file_type(mode_t st_mode)
{
    if (S_ISREG(st_mode)) {
        return REGULAR;
    }

    if (S_ISDIR(st_mode)) {
        return DIRECTORY;
    }

    if (S_ISCHR(st_mode)) {
        return CHAR_DEVICE;
    }

    if (S_ISBLK(st_mode)) {
        return BLOCK_DEVICE;
    }

    if (S_ISFIFO(st_mode)) {
        return FIFO;
    }

    if (S_ISLNK(st_mode)) {
        return SYM_LINK;
    }

    if (S_ISSOCK(st_mode)) {
        return SOCKET;
    }

    return UNKNOWN;
}

struct file_info *get_file_info(const char *file_path)
{
    assert(file_path != NULL);

    int fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return NULL;
    }

    struct stat file_stat;
    if (fstat(fd, &file_stat) < 0) {
        perror("fstat");
        if (close(fd) < 0) {
            perror("close");
        }
        return NULL;
    }

    struct file_info *file_info = calloc(1, sizeof(struct file_info));
    if (file_info == NULL) {
        fprintf(stderr, "Failed to allocate file info\n");
        if (close(fd) < 0) {
            perror("close");
        }
        return NULL;
    }

    if (close(fd) < 0) {
        perror("close");
    }

    file_info->path = split_string(file_path, "/");
    file_info->name = file_info->path->content[file_info->path->length - 1];

    file_info->type = get_file_type(file_stat.st_mode);

    file_info->owner = get_username(file_stat.st_uid);
    file_info->group = get_groupname(file_stat.st_gid);
    get_permissions(file_info->permissions, file_stat.st_mode);
    get_flags(file_info->flags, file_stat.st_mode);

    return file_info;
}

void free_file_info(struct file_info *file_info)
{
    if (file_info != NULL) {
        free_array(file_info->path);
    }

    free(file_info);
}


string_t fget_line(FILE *file)
{
    assert(file != NULL);

    string_t line = create_string(0);
    if (line == NULL) {
        fprintf(stderr, "Failed to create line\n");
        return NULL;
    }

    char c;
    while ((c = (char)fgetc(file)) != EOF) {
        line = append_char(line, c);
        if (c == '\n') {
            break;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "I/O error while reading file\n");
        free(line);
        return NULL;
    }

    return line;
}
