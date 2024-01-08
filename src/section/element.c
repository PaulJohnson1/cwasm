#include <section/element.h>

#include <stdlib.h>
#include <assert.h>

#include <pb.h>

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

#define element_instructions      \
    do                            \
    {                             \
        switch (self->mode_flags) \
        {                         \
        case 0:                   \
            expression;           \
            initialization;       \
            break;                \
        case 1:                   \
            element_kind;         \
            initialization;       \
            break;                \
        case 2:                   \
            table_index;          \
            expression;           \
            element_kind;         \
            initialization;       \
            break;                \
        case 3:                   \
            element_kind;         \
            initialization;       \
            break;                \
        case 4:                   \
            assert(0);            \
            break;                \
        case 5:                   \
            reference_type;       \
            expression;           \
            break;                \
        case 6:                   \
            assert(0);            \
            break;                \
        case 7:                   \
            reference_type;       \
            expression;           \
            break;                \
        }                         \
    } while (0)

int cwasm_section_element_write(struct cwasm_section_element *self, struct proto_bug *writer)
{
#define expression                                                                              \
    do                                                                                          \
    {                                                                                           \
        for (struct cwasm_instruction *i = self->instructions; i < self->instructions_end; i++) \
            cwasm_instruction_write(i, writer);                                                 \
    } while (0)

#define initialization                                                                      \
    do                                                                                      \
    {                                                                                       \
        proto_bug_write_varuint(writer, self->init_end - self->init, "element::init_size"); \
        for (uint64_t *i = self->init; i < self->init_end; i++)                              \
            proto_bug_write_varuint(writer, *i, "element::init");                           \
    } while (0)

#define table_index                                                                 \
    do                                                                              \
    {                                                                               \
        proto_bug_write_varuint(writer, self->table_index, "element::table_index"); \
    } while (0)

#define reference_type                                              \
    do                                                              \
    {                                                               \
        proto_bug_write_uint8(writer, self->type, "element::type"); \
    } while (0)

#define element_kind                                                 \
    do                                                               \
    {                                                                \
        proto_bug_write_varuint(writer, 0, "element::element_kind"); \
    } while (0)

    proto_bug_write_uint8(writer, self->mode_flags, "element::mode_flags");

    element_instructions;

#undef expression
#undef initialization
#undef table_index
#undef reference_type
#undef element_kind

    return cwasm_error_ok;
}

int cwasm_section_element_read(struct cwasm_section_element *self, struct proto_bug *reader)
{
#define expression                                                                                                            \
    do                                                                                                                        \
    {                                                                                                                         \
        int e = cwasm_instruction_read_vector(reader, &self->instructions, &self->instructions_end, &self->instructions_cap); \
        if (e)                                                                                                                \
            assert(0 == e);                                                                                                         \
    } while (0)

#define initialization                                                       \
    do                                                                       \
    {                                                                        \
        uint64_t max = proto_bug_read_varuint(reader, "element::init_size"); \
        self->init = malloc(max * sizeof *self->init);                       \
        self->init_cap = self->init_end = self->init + max;                  \
        for (uint64_t i = 0; i < max; i++)                                   \
            self->init[i] = proto_bug_read_varuint(reader, "element::init"); \
    } while (0)

#define table_index                                                                 \
    do                                                                              \
    {                                                                               \
        self->table_index = proto_bug_read_varuint(reader, "element::table_index"); \
    } while (0)

#define reference_type                                                                    \
    do                                                                                    \
    {                                                                                     \
        self->type = proto_bug_read_varuint(reader, "element::reference_type"); \
    } while (0)

#define element_kind                                             \
    do                                                           \
    {                                                            \
        proto_bug_read_varuint(reader, "element::element_kind"); \
    } while (0)

    self->mode_flags = 0b111 & proto_bug_read_uint8(reader, "element::mode_flags");

    element_instructions;

#undef expression
#undef initialization
#undef table_index
#undef reference_type
#undef element_kind

    return cwasm_error_ok;
}
