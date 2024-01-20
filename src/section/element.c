#include <section/element.h>

#include <assert.h>
#include <stdlib.h>

#include <pb.h>

#include <consts.h>
#include <log.h>
#include <section/code.h>

enum cwasm_element_segment_mode
{
    cwasm_element_segment_mode_active,
    cwasm_element_segment_mode_declarative,
    cwasm_element_segment_mode_passive
};

void cwasm_section_element_free(struct cwasm_section_element *self)
{
    for (struct cwasm_instruction_expression *i = self->expressions;
         i < self->expressions_end; i++)
        cwasm_instruction_expression_free(i);
    cwasm_instruction_expression_free(&self->expression);
    free(self->expressions);
    free(self->init);
}

#define element_instructions                                                   \
    switch (self->mode_flags)                                                  \
    {                                                                          \
    case 0:                                                                    \
        expression;                                                            \
        initialization;                                                        \
        break;                                                                 \
    case 1:                                                                    \
        element_kind;                                                          \
        initialization;                                                        \
        break;                                                                 \
    case 2:                                                                    \
        table_index;                                                           \
        expression;                                                            \
        element_kind;                                                          \
        initialization;                                                        \
        break;                                                                 \
    case 3:                                                                    \
        element_kind;                                                          \
        initialization;                                                        \
        break;                                                                 \
    case 4:                                                                    \
        expression;                                                            \
        expression_vector;                                                     \
        break;                                                                 \
    case 5:                                                                    \
        reference_type;                                                        \
        expression_vector;                                                     \
        break;                                                                 \
    case 6:                                                                    \
        table_index;                                                           \
        expression;                                                            \
        reference_type;                                                        \
        expression_vector;                                                     \
        break;                                                                 \
    case 7:                                                                    \
        reference_type;                                                        \
        expression_vector;                                                     \
        break;                                                                 \
    }

void cwasm_section_element_write(struct cwasm_section_element *self,
                                 struct proto_bug *writer)
{
#define expression                                                             \
    cwasm_instruction_expression_write(&self->expression, writer);

#define expression_vector                                                      \
    cwasm_log("write   elem seg elem expr count: %lu\n",                       \
              self->expressions_end - self->expressions);                      \
    for (struct cwasm_instruction_expression *i = self->expressions;           \
         i < self->expressions_end; i++)                                       \
        cwasm_instruction_expression_write(i, writer);

#define initialization                                                         \
    proto_bug_write_varuint(writer, self->init_end - self->init,               \
                            "element::init_size");                             \
    for (uint64_t *i = self->init; i < self->init_end; i++)                    \
        proto_bug_write_varuint(writer, *i, "element::init");

#define table_index                                                            \
    proto_bug_write_varuint(writer, self->table_index,                         \
                            "element::table_index");                           \
    cwasm_log("write   elem seg table_index: %lu\n", self->table_index);

#define reference_type                                                         \
    proto_bug_write_varuint(writer, self->type, "element::type");              \
    cwasm_log("write   elem seg type: %d\n", self->type);

#define element_kind                                                           \
    proto_bug_write_varuint(writer, 0, "element::element_kind");

    proto_bug_write_uint8(writer, self->mode_flags, "element::mode_flags");

    cwasm_log("write   begin elem seg: flags: %u\n", self->mode_flags);
    element_instructions;
    cwasm_log("write   end elem seg\n");

#undef expression
#undef expression_vector
#undef initialization
#undef table_index
#undef reference_type
#undef element_kind
}

void cwasm_section_element_read(struct cwasm_section_element *self,
                                struct proto_bug *reader)
{
#define expression cwasm_instruction_expression_read(&self->expression, reader);

#define expression_vector                                                      \
    do                                                                         \
    {                                                                          \
        uint64_t count = proto_bug_read_varuint(reader, "element::expr_vec::"  \
                                                        "count");              \
        cwasm_log("read    elem seg expr vec count: %lu\n", count);            \
        if (count)                                                             \
        {                                                                      \
            self->expressions = malloc(count * sizeof *self->expressions);     \
            self->expressions_end = self->expressions_cap =                    \
                self->expressions + count;                                      \
                                                                               \
            for (struct cwasm_instruction_expression *i = self->expressions;   \
                 i < self->expressions_end; i++)                               \
                cwasm_instruction_expression_read(i, reader);                  \
        }                                                                      \
    } while (0);

#define initialization                                                         \
    do                                                                         \
    {                                                                          \
        uint64_t max = proto_bug_read_varuint(reader, "element::init_size");   \
        self->init = malloc(max * sizeof *self->init);                         \
        self->init_cap = self->init_end = self->init + max;                    \
        for (uint64_t i = 0; i < max; i++)                                     \
            self->init[i] = proto_bug_read_varuint(reader, "element::init");   \
    } while (0);

#define table_index                                                            \
    self->table_index = proto_bug_read_varuint(reader, "element::table_"       \
                                                       "index");               \
    cwasm_log("read    elem seg table_index: %lu\n", self->table_index);

#define reference_type                                                         \
    self->type = proto_bug_read_uint8(reader, "element::reference_type");      \
    cwasm_log("read    elem seg type: %u\n", self->type);

#define element_kind proto_bug_read_varuint(reader, "element::element_kind");

    self->mode_flags =
        0b111 & proto_bug_read_uint8(reader, "element::mode_flags");

    cwasm_log("read    begin elem seg: flags: %d\n", self->mode_flags);
    element_instructions;
    cwasm_log("read    end elem seg\n");

#undef expression
#undef expression_vector
#undef initialization
#undef table_index
#undef reference_type
#undef element_kind
}
