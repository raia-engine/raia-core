//
// Created by dolphilia on 2022/12/10.
//

#ifndef RAIA_CORE_RAIA_DUKTAPE_H
#define RAIA_CORE_RAIA_DUKTAPE_H

#include <string.h>
#include "../wrapper/wrapper_duktape.h"
#include "../wrapper/wrapper_whereami.h"
#include "../static/static_duk_ctx.h"
#include "../static/static_arg.h"
#include "../duktape-module/duk_module_duktape.h"
#include "../static/static_plugin_loader.h"
#include "../meta/version.h"
#include "../util/util_dir.h"

void duktape_start(void);

#endif //RAIA_CORE_RAIA_DUKTAPE_H
