#include <section/data.h>

#include <assert.h>
#include <stdlib.h>

#include <pb.h>

#include <consts.h>
#include <log.h>
#include <section/code.h>

void cwasm_section_data_free(struct cwasm_section_data *self)
{
    cwasm_instruction_expression_free(&self->expression);
    free(self->initialization);
}

#define data_instructions                                                      \
    switch (self->mode)                                                        \
    {                                                                          \
    case 0:                                                                    \
        expression;                                                            \
        initialization;                                                        \
        break;                                                                 \
    case 1:                                                                    \
        initialization;                                                        \
        break;                                                                 \
    case 2:                                                                    \
        memory_index;                                                          \
        expression;                                                            \
        initialization;                                                        \
        break;                                                                 \
    };

void cwasm_section_data_write(struct cwasm_section_data *self,
                              struct proto_bug *writer)
{
#define initialization                                                         \
    for (uint8_t *i = self->initialization; i < self->initialization_end; i++) \
        proto_bug_write_uint8(writer, *i, "data::init");                       \
    cwasm_log("write   data seg init size: %lu\n",                             \
              self->initialization_end - self->initialization);

#define expression                                                             \
    cwasm_instruction_expression_write(&self->expression, writer);             \
    cwasm_log("write   data seg expr\n");

#define memory_index                                                           \
    proto_bug_write_uint8(writer, self->memory_index, "data::memory_index");   \
    cwasm_log("write   data memory index\n");

    proto_bug_write_varuint(writer, self->mode, "data::mode");
    cwasm_log("write   begin data seg: mode: %d\n", self->mode);
    data_instructions;
    cwasm_log("write   end data seg\n");

#undef initialization
#undef expression
#undef memory_index
}

void cwasm_section_data_read(struct cwasm_section_data *self,
                             struct proto_bug *reader)
{
#define initialization                                                         \
    do                                                                         \
    {                                                                          \
        uint64_t max = proto_bug_read_varuint(reader, "data::init_size");      \
        cwasm_log("read    data seg init size: %lu\n", max);                   \
        for (uint64_t i = 0; i < max; i++)                                     \
        {                                                                      \
            cwasm_vector_grow(uint8_t, self->initialization);                  \
            *self->initialization_end++ =                                      \
                proto_bug_read_uint8(reader, "data::init");                    \
        }                                                                      \
    } while (0);

#define expression                                                             \
    cwasm_instruction_expression_read(&self->expression, reader);              \
    cwasm_log("read    data seg expr\n");

#define memory_index                                                           \
    proto_bug_read_uint8(reader, "data::memory_index");                        \
    cwasm_log("read    data seg memory index: %d\n", self->memory_index);

    self->mode = proto_bug_read_varuint(reader, "data::mode");
    cwasm_log("read    begin data seg: mode: %d\n", self->mode);
    data_instructions;
    cwasm_log("read    end data seg\n");

#undef initialization
#undef expression
#undef memory_index
}
