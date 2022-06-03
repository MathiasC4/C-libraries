//
// Created by Matyas Cupr on 06/05/2022.
//

#include "user_handling.h"


char *get_username(uid_t uid)
{
    struct passwd *passwd;
    errno = 0;
    if ((passwd = getpwuid(uid)) == NULL) {
        if (errno != 0) {
            perror("getpwuid");
        }
        return NULL;
    }

    return passwd->pw_name;
}

char *get_groupname(gid_t gid)
{
    struct group *group;
    errno = 0;
    if ((group = getgrgid(gid)) == NULL) {
        if (errno != 0) {
            perror("getgrgid");
        }
        return NULL;
    }

    return group->gr_name;
}

static void parse_perms(char perms[3], mode_t st_mode)
{
    mode_t READ = 04;
    mode_t WRITE = 02;
    mode_t EXECUTE = 01;

    if (st_mode & READ) {
        perms[0] = 'r';
    } else {
        perms[0] = '-';
    }

    if (st_mode & WRITE) {
        perms[1] = 'w';
    } else {
        perms[1] = '-';
    }

    if (st_mode & EXECUTE) {
        perms[2] = 'x';
    } else {
        perms[2] = '-';
    }
}

char *get_permissions(char permissions[9 + 1], mode_t st_mode)
{
    parse_perms(permissions + 0, st_mode & S_IRWXU >> 6);
    parse_perms(permissions + 3, st_mode & S_IRWXG >> 3);
    parse_perms(permissions + 6, st_mode & S_IRWXG >> 0);

    return permissions;
}

char *get_flags(char flags[3 + 1], mode_t st_mode)
{
    if (st_mode & S_ISUID) {
        flags[0] = 's';
    } else {
        flags[0] = '-';
    }

    if (st_mode & S_ISGID) {
        flags[1] = 's';
    } else {
        flags[1] = '-';
    }

    if (st_mode & S_ISVTX) {
        flags[2] = 't';
    } else {
        flags[2] = '-';
    }

    return flags;
}