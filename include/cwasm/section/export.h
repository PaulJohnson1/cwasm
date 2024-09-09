#pragma once

#include <stdint.h>

#include <cwasm/type.h>
#include <cwasm/util.h>

struct proto_bug;

struct cwasm_section_export
{

    uint64_t index;
    char *name;
    uint8_t type;
};

extern void cwasm_section_export_free(struct cwasm_section_export *);
extern void cwasm_section_export_write(struct cwasm_section_export *,
                                       struct proto_bug *);
extern void cwasm_section_export_read(struct cwasm_section_export *,
                                      struct proto_bug *);
