#pragma once

#include <stdint.h>

<<<<<<< Updated upstream:src/section/export.h
#include <type.h>
=======
#include <cwasm/type.h>
#include <cwasm/util.h>
>>>>>>> Stashed changes:include/cwasm/section/export.h

struct proto_bug;

struct cwasm_section_export
{
    char *name;
};

extern void cwasm_section_export_free(struct cwasm_section_export *);
extern int cwasm_section_export_write(struct cwasm_section_export *,
                                      struct proto_bug *);
extern int cwasm_section_export_read(struct cwasm_section_export *,
                                     struct proto_bug *);
