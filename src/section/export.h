#pragma once

#include <stdint.h>

#include <type.h>

struct proto_bug;

struct cwasm_section_export
{
    char *name;
    struct type_external type;
};

extern void cwasm_section_export_free(struct cwasm_section_export *);
extern int cwasm_section_export_write(struct cwasm_section_export *, struct proto_bug *);
extern int cwasm_section_export_read(struct cwasm_section_export *, struct proto_bug *);
