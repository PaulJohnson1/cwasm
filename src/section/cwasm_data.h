#pragma once

#include <stdint.h>

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

    struct cwasm_instruction *offset;
    struct cwasm_instruction *offset_end;
    struct cwasm_instruction *offset_cap;
    uint8_t *initialization;
    uint8_t *initialization_end;
    uint8_t *initialization_cap;
};

extern void cwasm_section_data_free(struct cwasm_section_data *);
extern int cwasm_section_data_write(struct cwasm_section_data *, struct proto_bug *);
extern int cwasm_section_data_read(struct cwasm_section_data *, struct proto_bug *);
