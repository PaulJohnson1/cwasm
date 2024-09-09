#pragma once

#include <stdint.h>

#include <cwasm/type.h>

struct proto_bug;

struct cwasm_section_custom
{
    char *name;

    uint8_t *bytes;
    uint8_t *bytes_end;
    uint8_t *bytes_cap;
};

extern void cwasm_section_custom_free(struct cwasm_section_custom *);
extern void cwasm_section_custom_write(struct cwasm_section_custom *,
                                       struct proto_bug *);
extern void cwasm_section_custom_read(struct cwasm_section_custom *,
                                      struct proto_bug *);
