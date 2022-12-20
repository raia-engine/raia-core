//
// Created by dolphilia on 2022/12/10.
//

#include "raia_duktape.h"

#define YES_RETURN_VALUE 1
#define NO_RETURN_VALUE 0 // = undefined

static duk_ret_t regist_raia_c_puts(duk_context *ctx) {
    int ret = puts(duk_to_string(ctx, 0));
    duk_push_number(ctx, ret);
    return YES_RETURN_VALUE;
}

static duk_ret_t regist_raia_lib_init(duk_context *ctx) {
    init_plugin_loader();
    return NO_RETURN_VALUE;
}

static duk_ret_t regist_raia_lib_open(duk_context *ctx) {
    const char *dll_file = duk_to_string(ctx, 0);
    open_plugin(dll_file);
    return NO_RETURN_VALUE;
}

static duk_ret_t regist_raia_lib_close(duk_context *ctx) {
    close_plugin();
    return NO_RETURN_VALUE;
}

static duk_ret_t regist_raia_lib_close_all(duk_context *ctx) {
    close_all_plugin();
    return NO_RETURN_VALUE;
}

static duk_ret_t regist_raia_lib_addGlobalFunc(duk_context *ctx) {
    const char *dll_func_name = duk_to_string(ctx, 0);
    const char *regist_func_name = duk_to_string(ctx, 1);
    int nargs = (int) duk_to_number(ctx, 2);
    duk_ret_t (*p_func)(duk_context *ctx) = add_plugin_func(ctx, dll_func_name);
    duk_push_c_function(ctx, p_func, nargs);
    duk_put_global_string(ctx, regist_func_name);
    return NO_RETURN_VALUE;
}

static duk_ret_t regist_raia_lib_getFunc(duk_context *ctx) {
    const char *dll_func_name = duk_to_string(ctx, 0);
    int nargs = (int) duk_to_number(ctx, 1);
    duk_ret_t (*p_func)(duk_context *ctx) = add_plugin_func(ctx, dll_func_name);
    duk_push_c_function(ctx, p_func, nargs);
    return 1;
}

static void regist_functions(duk_context *ctx) {
    regist_func(ctx, regist_raia_c_puts, "_raia_c_puts", 1);
    regist_func(ctx, regist_raia_lib_init, "_raia_lib_init", 0);
    regist_func(ctx, regist_raia_lib_open, "_raia_lib_open", 1);
    regist_func(ctx, regist_raia_lib_close, "_raia_lib_close", 0);
    regist_func(ctx, regist_raia_lib_close_all, "_raia_lib_close_all", 0);
    regist_func(ctx, regist_raia_lib_addGlobalFunc, "_raia_lib_add_global_func", 3);
    regist_func(ctx, regist_raia_lib_getFunc, "_raia_lib_get_func", 2);
}

static void regist_objects(duk_context *ctx) {
    char *objects =
            "var print = function(a){_raia_c_puts(a);};"
            "var Raia = {"
            "    Lib: {"
            "        init: function(){_raia_lib_init();},"
            "        open: function(a){_raia_lib_open(a);},"
            "        close: function(){_raia_lib_close();},"
            "        closeAll: function(){_raia_lib_close_all();},"
            "        addGlobalFunc: function(a,b,c){_raia_lib_add_global_func(a,b,c);},"
            "        getFunc: function(a,b){return _raia_lib_get_func(a,b);},"
            "    },"
            "};";
    duk_eval_string(ctx, objects);
}

static void regist_elements(duk_context *ctx) {
    regist_functions(ctx);
    regist_objects(ctx);
}

static void enable_module(duk_context *ctx) {
    char *code =
            "Duktape.modSearch = function (id, require, exports, module) {"
            "    var filename = 'modules/' + id + '.js';"
            "    var src = IO.loadStringFilename(filename);"
            "    if (typeof src === 'string') {"
            "        print('loaded ECMAScript:', filename);"
            "    } else {"
            "        throw new Error('module not found: ' + id);"
            "    }"
            "    return src;"
            "};";
    duk_eval_string(ctx, code);
}

void duktape_start(void) {
    duk_context *ctx = get_duk_ctx();
    regist_elements(ctx);
    duk_module_duktape_init(ctx);
    enable_module(ctx);
    load_script_filename(ctx, "startup.js");
    duk_eval(ctx);
    duk_destroy_heap(ctx);
}
