//
// Created by dolphilia on 2022/12/10.
//

#include "wrapper_duktape.h"

void load_script_filename(duk_context *ctx, const char *filename) {
    char *script = load_string_filename(filename); //ファイルを読み込む
    duk_push_string(ctx, script);
    free(script);
}

void regist_func(duk_context *ctx, duk_c_function func, const char *str, int nargs) {
    duk_push_c_function(ctx, func, nargs);
    duk_put_global_string(ctx, str);
}
