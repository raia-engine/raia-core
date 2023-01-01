//
// Created by dolphilia on 2022/12/23.
//

#ifndef RAIA_CORE_UTIL_DIR_H
#define RAIA_CORE_UTIL_DIR_H

#include <stdio.h>

#include "../meta/platform.h"
#include "limits.h"

#ifdef __WINDOWS__
#include <windows.h>
#include <tchar.h>
#else
#include <unistd.h>
#endif


#ifdef __MACOS__
#include <mach-o/dyld.h>
#endif

//char * get_exe_path();
char *get_cur_path();

#endif //RAIA_CORE_UTIL_DIR_H
