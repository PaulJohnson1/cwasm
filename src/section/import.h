#pragma once

#include <consts.h>
#include <type.h>

union cwasm_section_description
{
    uint64_t table_index; 
    struct cwasm_type_table table;
    struct cwasm_type_memory memory;
    struct cwasm_type_global global;
};

struct cwasm_section_import
{
    char *module;
    char *name;
    enum cwasm_external_type type;
    union cwasm_section_description description;
};

extern void cwasm_section_import_free(struct cwasm_section_import *);
extern int cwasm_section_import_write(struct cwasm_section_import *,
                                      struct proto_bug *);
extern int cwasm_section_import_read(struct cwasm_section_import *,
                                     struct proto_bug *);
