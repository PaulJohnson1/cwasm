#pragma once

#include <stdint.h>

struct proto_bug;

struct cwasm_section_element
{
    uint64_t table_index;

    uint8_t *init;
    uint8_t *init_end;
    uint8_t *init_cap;

    struct cwasm_instruction *instructions;
    struct cwasm_instruction *instructions_end;
    struct cwasm_instruction *instructions_cap;

    struct cwasm_instruction *initialization;
    struct cwasm_instruction *initialization_end;
    struct cwasm_instruction *initialization_cap;

    uint8_t mode_flags;
    uint8_t type;
};

extern void cwasm_section_element_free(struct cwasm_section_element *);
extern int cwasm_section_element_write(struct cwasm_section_element *, struct proto_bug *);
extern int cwasm_section_element_read(struct cwasm_section_element *, struct proto_bug *);
