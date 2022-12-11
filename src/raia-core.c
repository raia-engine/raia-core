//
// Created by dolphilia on 2022/12/10.
//

#include "raia-core.h"
#include "static/static_plugin_loader.h"

int main() {
    init_plugin_loader();
    init_duk_ctx();
    duktape_start();
    return 0;
}