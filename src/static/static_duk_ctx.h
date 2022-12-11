//
// Created by dolphilia on 2022/12/10.
//

#ifndef RAIA_CORE_STATIC_DUK_CTX_H
#define RAIA_CORE_STATIC_DUK_CTX_H

#include <stdio.h>
#include "../duktape/duktape.h"

void init_duk_ctx(void);

duk_context *get_duk_ctx(void);

#endif //RAIA_CORE_STATIC_DUK_CTX_H
