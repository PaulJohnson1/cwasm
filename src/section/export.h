#pragma once

#include <stdint.h>

#include <type.h>

struct proto_bug;

struct cwasm_section_export
{
    char *name;
};

extern void cwasm_section_export_free(struct cwasm_section_export *);
extern void cwasm_section_export_write(struct cwasm_section_export *,
                                      struct proto_bug *);
extern void cwasm_section_export_read(struct cwasm_section_export *,
                                     struct proto_bug *);
