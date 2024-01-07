#pragma once

#include <stdint.h>

#include <section/cwasm_code.h>

struct proto_bug;

struct cwasm_section_global
{
    uint64_t type;
    struct cwasm_instruction *instructions;
    struct cwasm_instruction *instructions_end;
    struct cwasm_instruction *instructions_cap;
};

extern void cwasm_section_global_free(struct cwasm_section_global *);
extern int cwasm_section_global_write(struct cwasm_section_global *,
                                      struct proto_bug *);
extern int cwasm_section_global_read(struct cwasm_section_global *,
                                     struct proto_bug *);
