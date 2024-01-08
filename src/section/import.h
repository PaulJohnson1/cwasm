#pragma once

#include <type.h>

struct cwasm_section_import
{
    struct cwasm_type_external type;
    char *module;
    char *name;
};

extern void cwasm_section_import_free(struct cwasm_section_import *);
extern int cwasm_section_import_write(struct cwasm_section_import *,
                                      struct proto_bug *);
extern int cwasm_section_import_read(struct cwasm_section_import *,
                                     struct proto_bug *);
