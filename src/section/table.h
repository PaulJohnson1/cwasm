#pragma once

#include <stdint.h>

struct proto_bug;

struct cwasm_section_table
{
    uint64_t type;
    uint64_t min;
    uint64_t max;
};

extern void cwasm_section_table_free(struct cwasm_section_table *);
extern void cwasm_section_table_write(struct cwasm_section_table *,
                                      struct proto_bug *);
extern void cwasm_section_table_read(struct cwasm_section_table *,
                                     struct proto_bug *);
