//
// Created by dolphilia on 2022/12/10.
//

#ifndef RAIA_CORE_WRAPPER_DUKTAPE_H
#define RAIA_CORE_WRAPPER_DUKTAPE_H

#include "../duktape/duktape.h"
#include "../util/util_io.h"

void load_script_filename(duk_context *ctx, const char *filename);
void regist_func(duk_context *ctx, duk_c_function func, const char *str, int nargs);

#endif //RAIA_CORE_WRAPPER_DUKTAPE_H