#pragma once

#include <stdint.h>

#include <type.h>

struct proto_bug;

struct cwasm_section_data_count
{
    uint64_t count;
};

extern void cwasm_section_data_count_free(struct cwasm_section_data_count *);
extern void cwasm_section_data_count_write(struct cwasm_section_data_count *,
                                           struct proto_bug *);
extern void cwasm_section_data_count_read(struct cwasm_section_data_count *,
                                          struct proto_bug *);
