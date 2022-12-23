//
// Created by dolphilia on 2022/12/23.
//

#include "wrapper_whereami.h"

char *get_executable_path(void) {
    char *path = NULL;
    int length, dirname_length;
    length = wai_getExecutablePath(NULL, 0, NULL);
    path = (char *) malloc(length + 1);
    wai_getExecutablePath(path, length, &dirname_length);
    path[length] = '\0';
    return path;
}