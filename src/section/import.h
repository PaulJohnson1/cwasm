#pragma once

#include <consts.h>
#include <type.h>

struct cwasm_section_import
{
    union
    {
        uint64_t table_index;
        struct cwasm_type_memory memory;
        struct cwasm_type_table table;
        struct cwasm_type_global global;
    };
    char *module;
    char *name;

    uint8_t type;
};

extern void cwasm_section_import_free(struct cwasm_section_import *);
extern void cwasm_section_import_write(struct cwasm_section_import *,
                                       struct proto_bug *);
extern void cwasm_section_import_read(struct cwasm_section_import *,
                                      struct proto_bug *);
