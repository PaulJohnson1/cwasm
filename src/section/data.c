#include <section/data.h>

#include <assert.h>
#include <stdlib.h>

#include <pb.h>

#include <consts.h>
#include <section/code.h>

void cwasm_section_data_free(struct cwasm_section_data *self)
{
    for (struct cwasm_instruction *i = self->offset; i < self->offset_end; i++)
        cwasm_instruction_free(i);
    free(self->offset);
    free(self->initialization);
}

int cwasm_section_data_write(struct cwasm_section_data *self,
                             struct proto_bug *writer)
{
    return cwasm_error_ok;
}

int cwasm_section_data_read(struct cwasm_section_data *self,
                            struct proto_bug *reader)
{
#define READ_INIT                                                              \
    uint64_t max = proto_bug_read_varuint(reader, "data::init_size");          \
    for (uint64_t i = 0; i < max; i++)                                         \
    {                                                                          \
        if (self->initialization_end >= self->initialization_cap)              \
        {                                                                      \
            uint64_t capacity =                                                \
                self->initialization_cap - self->initialization;               \
            uint8_t *new_data =                                                \
                realloc(self->initialization,                                  \
                        (capacity * 2 + 1) * sizeof *self->initialization);    \
            uint8_t *new_data_cap = new_data + capacity * 2 + 1;               \
            self->initialization = new_data;                                   \
            self->initialization_end = new_data + capacity;                    \
            self->initialization_cap = new_data_cap;                           \
        }                                                                      \
        *self->initialization_end++ =                                          \
            proto_bug_read_uint8(reader, "data::init");                        \
    }

    self->mode = proto_bug_read_uint8(reader, "data::mode");
    switch (self->mode)
    {
    case cwasm_data_segment_mode_active:
    {
        self->memory_index = 0;
        cwasm_instruction_read_vector(reader, &self->offset, &self->offset_end,
                                      &self->offset_cap);
        READ_INIT
        break;
    }
    case cwasm_data_segment_mode_passive:
    {
        READ_INIT
        break;
    }
    case cwasm_data_segment_mode_active_with_memory_index:
    {
        self->memory_index =
            proto_bug_read_varuint(reader, "data::memory_index");
        cwasm_instruction_read_vector(reader, &self->offset, &self->offset_end,
                                      &self->offset_cap);
        READ_INIT
        break;
    }
    default:
        assert(0);
        break;
    }
    return cwasm_error_ok;
}
