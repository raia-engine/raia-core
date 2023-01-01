//
// Created by dolph on 2022/12/31.
//

#ifndef RAIA_CORE_UTIL_STRING_H
#define RAIA_CORE_UTIL_STRING_H

#include "../meta/platform.h"
#ifdef __WINDOWS__
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>

char *replace_char(char* str, const char before, const char after);
char* replace(char* s, const char* before, const char* after);
void strchg(char *buf, const char *str1, const char *str2);


#endif //RAIA_CORE_UTIL_STRING_H
