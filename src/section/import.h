#pragma once

#include <consts.h>
#include <type.h>

struct cwasm_section_import
{
    char *module;
    char *name;
    struct cwasm_type_description description;
};

extern void cwasm_section_import_free(struct cwasm_section_import *);
extern void cwasm_section_import_write(struct cwasm_section_import *,
                                      struct proto_bug *);
extern void cwasm_section_import_read(struct cwasm_section_import *,
                                     struct proto_bug *);
