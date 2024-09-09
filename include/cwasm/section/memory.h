#pragma once

#include <stdint.h>
#include <cwasm/type.h>

struct proto_bug;

struct cwasm_section_memory
{
    struct cwasm_type_limit limit;
};

extern void cwasm_section_memory_free(struct cwasm_section_memory *);
extern void cwasm_section_memory_write(struct cwasm_section_memory *,
                                       struct proto_bug *);
extern void cwasm_section_memory_read(struct cwasm_section_memory *,
                                      struct proto_bug *);
