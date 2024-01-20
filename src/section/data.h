#pragma once

#include <stdint.h>

#include <section/code.h>
#include <vector.h>

struct proto_bug;

struct cwasm_section_data
{
    struct cwasm_instruction_expression expression;
    cwasm_vector_declare(uint8_t, initialization);
    uint8_t memory_index;
    uint8_t mode;
};

extern void cwasm_section_data_free(struct cwasm_section_data *);
extern void cwasm_section_data_write(struct cwasm_section_data *,
                                     struct proto_bug *);
extern void cwasm_section_data_read(struct cwasm_section_data *,
                                    struct proto_bug *);
