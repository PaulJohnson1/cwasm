#pragma once

#ifdef WASM_BUILD
#include <emscripten.h>
#define CWASM_EXPORT EMSCRIPTEN_KEEPALIVE
#else
#define CWASM_EXPORT
#endif
