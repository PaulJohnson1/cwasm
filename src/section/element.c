#include <section/element.h>

#include <pb.h>

#include <stdlib.h>

#include <section/code.h>
#include <consts.h>

enum cwasm_element_segment_mode
{
    cwasm_element_segment_mode_active,
    cwasm_element_segment_mode_declarative,
    cwasm_element_segment_mode_passive
};

void cwasm_section_element_free(struct cwasm_section_element *self)
{
    for (struct cwasm_instruction *i = self->instructions; i < self->instructions_end; i++)
        cwasm_instruction_free(i);
    free(self->instructions);
    free(self->initialization);
}

int cwasm_section_element_write(struct cwasm_section_element *self, struct proto_bug *writer)
{
    proto_bug_write_uint8(writer, self->mode_flags, "element::mode_flags");

    enum cwasm_element_segment_mode mode;

    if ((self->mode_flags & 0b1) == 0)
        mode = cwasm_element_segment_mode_active;
    else if ((self->mode_flags & 0b11) == 0b11)
        mode = cwasm_element_segment_mode_declarative;
    else
        mode = cwasm_element_segment_mode_passive;

    if ((self->mode_flags & 0b10) == 0b10)
        proto_bug_write_varuint(writer, self->table_index, "element::table_index");
    if (mode == cwasm_element_segment_mode_active)
        for (struct cwasm_instruction *i = self->instructions; i < self->instructions_end; i++)
            cwasm_instruction_write(i, writer);

    if ((self->mode_flags & 0b11) != 0)
        proto_bug_write_uint8(writer, self->type, "element::type");
    if ((self->mode_flags & 0b100) == 0)
    {
        proto_bug_write_varuint(writer, self->init_end - self->init, "element::init_size");
        for (uint8_t *i = self->init; i < self->init_end; i++)
            proto_bug_write_varuint(writer, *i, "element::init");
    }
    else
        for (struct cwasm_instruction *i = self->instructions; i < self->instructions_end; i++)
            cwasm_instruction_write(i, writer);

    return cwasm_error_ok;
}

int cwasm_section_element_read(struct cwasm_section_element *self, struct proto_bug *reader)
{
    self->mode_flags = 0b111 & proto_bug_read_uint8(reader, "element::mode_flags");

    enum cwasm_element_segment_mode mode;

    if ((self->mode_flags & 0b1) == 0)
        mode = cwasm_element_segment_mode_active;
    else if ((self->mode_flags & 0b11) == 0b11)
        mode = cwasm_element_segment_mode_declarative;
    else
        mode = cwasm_element_segment_mode_passive;

    if ((self->mode_flags & 0b10) == 0b10)
        self->table_index = proto_bug_read_varuint(reader, "element::table_index");
    if (mode == cwasm_element_segment_mode_active)
    {
        int e = cwasm_instruction_read_vector(reader, &self->instructions, &self->instructions_end, &self->instructions_cap);
        if (e)
            return e;
    }

    if ((self->mode_flags & 0b11) != 0)
        self->type = proto_bug_read_uint8(reader, "element::type");

    if ((self->mode_flags & 0b100) == 0)
    {
        uint64_t max = proto_bug_read_varuint(reader, "element::init_size");
        self->init = malloc(max * sizeof *self->init);
        self->init_cap = self->init_end = self->init + max;
        for (uint64_t i = 0; i < max; i++)
            self->init[i] = proto_bug_read_varuint(reader, "element:;init");
    }
    else
    {
        int e = cwasm_instruction_read_vector(reader, &self->initialization, &self->initialization_end, &self->initialization_cap);
        if (e)
            return e;
    }
    return cwasm_error_ok;
}
