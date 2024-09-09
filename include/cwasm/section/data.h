#pragma once

#include <stdint.h>

#include <cwasm/section/code.h>
#include <cwasm/vector.h>

struct proto_bug;

enum cwasm_data_segment_mode
{
    cwasm_data_segment_mode_active,
    cwasm_data_segment_mode_passive,
    cwasm_data_segment_mode_active_with_memory_index
};

struct cwasm_section_data
{
    enum cwasm_data_segment_mode mode;
    uint64_t memory_index;

    struct cwasm_instruction_expression offset;
    cwasm_vector_declare(uint8_t, initialization);
};

extern void cwasm_section_data_free(struct cwasm_section_data *);
extern int cwasm_section_data_write(struct cwasm_section_data *,
                                    struct proto_bug *);
extern int cwasm_section_data_read(struct cwasm_section_data *,
                                   struct proto_bug *);
