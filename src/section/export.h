#pragma once

#include <stdint.h>

#include <util.h>
#include <type.h>

struct proto_bug;

struct cwasm_section_export
{

    union
    {
        uint64_t table_index;
        uint64_t memory_index;
        struct cwasm_type_table table;
        struct cwasm_type_global global;
    };
    char *name;
    uint8_t type;
};

extern void cwasm_section_export_free(struct cwasm_section_export *);
extern void cwasm_section_export_write(struct cwasm_section_export *,
                                       struct proto_bug *);
extern void cwasm_section_export_read(struct cwasm_section_export *,
                                      struct proto_bug *);
