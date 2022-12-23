//
// Created by dolphilia on 2022/12/23.
//

#include "static_arg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _argc;
static char **_argv;

void init_arg(int argc, char *argv[]) {
    _argc = argc;
    _argv = argv;
}

int get_argc() {
    return _argc;
}

char **get_argv() {
    return _argv;
}