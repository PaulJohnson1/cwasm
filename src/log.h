#pragma once

#ifdef cwasm_built_in_log
#include <stdio.h>
#define cwasm_log(...) fprintf(stderr, __VA_ARGS__)
#endif

#ifndef cwasm_log
#define cwasm_log(...) 0
#endif
