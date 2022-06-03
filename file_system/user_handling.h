//
// Created by Matyas Cupr on 06/05/2022.
//

#ifndef USER_HANDLING_H
#define USER_HANDLING_H

#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


char *get_username(uid_t uid);

char *get_groupname(gid_t gid);

char *get_permissions(char permissions[9 + 1], mode_t st_mode);

char *get_flags(char flags[3 + 1], mode_t st_mode);


#endif //USER_HANDLING_H
