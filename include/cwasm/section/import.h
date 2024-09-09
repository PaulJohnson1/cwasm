#pragma once

<<<<<<< Updated upstream:src/section/import.h
#include <consts.h>
#include <type.h>
=======
#include <cwasm/consts.h>
#include <cwasm/type.h>
#include <cwasm/util.h>
>>>>>>> Stashed changes:include/cwasm/section/import.h

struct cwasm_section_import
{
    char *module;
    char *name;
    struct cwasm_type_description description;
};

extern void cwasm_section_import_free(struct cwasm_section_import *);
extern int cwasm_section_import_write(struct cwasm_section_import *,
                                      struct proto_bug *);
extern int cwasm_section_import_read(struct cwasm_section_import *,
                                     struct proto_bug *);
