#pragma once

#include <stdint.h>

struct proto_bug;

struct cwasm_section_memory
{
    uint64_t min;
    uint64_t max;
};

extern void cwasm_section_memory_free(struct cwasm_section_memory *);
extern int cwasm_section_memory_write(struct cwasm_section_memory *, struct proto_bug *);
extern int cwasm_section_memory_read(struct cwasm_section_memory *, struct proto_bug *);
