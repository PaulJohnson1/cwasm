#pragma once

#include <stdint.h>

struct proto_bug;

struct cwasm_section_function
{
    uint64_t type_index;
};

extern void cwasm_section_function_free(struct cwasm_section_function *);
extern int cwasm_section_function_write(struct cwasm_section_function *,
                                        struct proto_bug *);
extern int cwasm_section_function_read(struct cwasm_section_function *,
                                       struct proto_bug *);
