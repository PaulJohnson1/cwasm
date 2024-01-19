#pragma once

#include <stdint.h>

#include <section/code.h>
#include <vector.h>

struct proto_bug;

struct cwasm_section_element
{
    struct cwasm_instruction_expression expression;
    uint64_t table_index;
    uint64_t type;
    cwasm_vector_declare(uint64_t, init);
    cwasm_vector_declare(struct cwasm_instruction_expression, expressions);
    uint8_t mode_flags;
};

extern void cwasm_section_element_free(struct cwasm_section_element *);
extern void cwasm_section_element_write(struct cwasm_section_element *,
                                       struct proto_bug *);
extern void cwasm_section_element_read(struct cwasm_section_element *,
                                      struct proto_bug *);
