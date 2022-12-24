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

int get_argv_length(char argc, char *argv[]) {
    int len = 0;
    for (int i = 0; i < argc; i += 1) {
        if (i > 0) {
            len += 1; // ,
        }
        len += strlen(argv[i]);
        len += 2; // ""
    }
    return len;
}

char *get_argv_joined(int argc, char *argv[], int len) {
    char *str = (char *)malloc(sizeof(char) * len);
    for (int i = 0; i < argc; i += 1) {
        if (i > 0) {
            strcat(str, ",");
        }
        strcat(str, "\"");
        strcat(str, argv[i]);
        strcat(str, "\"");
    }
    return str;
}

static duk_ret_t regist_raia_context_init(duk_context *ctx) {
    int argc = get_argc();
    char **argv = get_argv();
    int argv_len = get_argv_length(argc, argv);
    char *argv_joined = get_argv_joined(argc, argv, argv_len);
    char* path_exe = get_executable_path();
    char json[500];

    sprintf(json,
            "{"
            "    \"version\": {"
            "        \"core\": \"%s\""
            "    },"
            "    \"path\": {"
            "        \"exe\": \"%s\""
            "    },"
            "    \"arg\": {"
            "        \"c\": %d,"
            "        \"v\": [%s]"
            "    }"
            "}",
            RAIA_CORE_VERSION,
            path_exe,
            get_argc(),
            argv_joined
            );
    free(path_exe);
    free(argv_joined);

    duk_push_string(ctx, json);
    duk_json_decode(ctx, -1);
    return 1;
}

static duk_ret_t regist_raia_lib_init(duk_context *ctx) {
    init_plugin_loader();
    return NO_RETURN_VALUE;
}

static duk_ret_t regist_raia_lib_open(duk_context *ctx) {
    const char *dll_file = duk_to_string(ctx, 0);
#ifdef __WINDOWS__
    const char *extension = "dll";
#endif
#ifdef __MACOS__
    const char *extension = "dylib";
#endif
#ifdef __LINUX__
    const char *extension = "so";
#endif
    char dll_file_extension[500];
    sprintf(dll_file_extension, "%s.%s", dll_file, extension);
    open_plugin(dll_file_extension);
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

static duk_ret_t regist_raia_lib_func_global(duk_context *ctx) {
    const char *dll_func_name = duk_to_string(ctx, 0);
    const char *regist_func_name = duk_to_string(ctx, 1);
    int nargs = (int) duk_to_number(ctx, 2);
    duk_ret_t (*p_func)(duk_context *ctx) = add_plugin_func(ctx, dll_func_name);
    duk_push_c_function(ctx, p_func, nargs);
    duk_put_global_string(ctx, regist_func_name);
    return NO_RETURN_VALUE;
}

static duk_ret_t regist_raia_lib_func(duk_context *ctx) {
    const char *dll_func_name = duk_to_string(ctx, 0);
    int nargs = (int) duk_to_number(ctx, 1);
    duk_ret_t (*p_func)(duk_context *ctx) = add_plugin_func(ctx, dll_func_name);
    duk_push_c_function(ctx, p_func, nargs);
    return 1;
}

static void regist_functions(duk_context *ctx) {
    regist_func(ctx, regist_raia_c_puts, "_raia_c_puts", 1);
    regist_func(ctx, regist_raia_context_init, "_raia_context_init", 0);
    regist_func(ctx, regist_raia_lib_init, "_raia_lib_init", 0);
    regist_func(ctx, regist_raia_lib_open, "_raia_lib_open", 1);
    regist_func(ctx, regist_raia_lib_close, "_raia_lib_close", 0);
    regist_func(ctx, regist_raia_lib_close_all, "_raia_lib_close_all", 0);
    regist_func(ctx, regist_raia_lib_func_global, "_raia_lib_func_global", 3);
    regist_func(ctx, regist_raia_lib_func, "_raia_lib_func", 2);
}

static void regist_objects(duk_context *ctx) {
    char *objects =
            //  assert
            //  do_file
            // abort
            // exit
            // exec
            // sleep
            "var print = function(a){_raia_c_puts(a);};"
            "var Raia = {"
            "    Context: {"
            "        init: function(){return _raia_context_init();},"
            "    },"
            "    Lib: {"
            "        init: function(){_raia_lib_init();},"
            "        open: function(a){_raia_lib_open(a);},"
            "        close: function(){_raia_lib_close();},"
            "        closeAll: function(){_raia_lib_close_all();},"
            "        funcGlobal: function(a,b,c){_raia_lib_func_global(a,b,c);},"
            "        func: function(a,b){return _raia_lib_func(a,b);},"
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
