#include <cwasm/section/data.h>

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

#include <cwasm/pb.h>

#include <cwasm/consts.h>
#include <cwasm/log.h>
#include <cwasm/section/code.h>

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
                              struct proto_bug *pb)
{
#define initialization                                                         \
    proto_bug_write_varuint(pb,                                                \
                            self->initialization_end - self->initialization,   \
                            "data::init_size");                                \
    for (uint8_t *i = self->initialization; i < self->initialization_end; i++) \
        proto_bug_write_uint8(pb, *i, "data::init");                           \
    cwasm_log("write @%08lx  data seg init size: %" PRIuPTR "\n",              \
              proto_bug_get_total_size(pb),                                    \
              self->initialization_end - self->initialization);

#define expression                                                             \
    cwasm_instruction_expression_write(&self->expression, pb);                 \
    cwasm_log("write @%08lx  data seg expr\n", proto_bug_get_total_size(pb));

#define memory_index                                                           \
    proto_bug_write_uint8(pb, self->memory_index, "data::memory_index");       \
    cwasm_log("write @%08lx  data memory index\n",                             \
              proto_bug_get_total_size(pb));

    proto_bug_write_varuint(pb, self->mode, "data::mode");
    cwasm_log("write @%08lx  begin data seg: mode: %d\n",
              proto_bug_get_total_size(pb), self->mode);
    data_instructions;
    cwasm_log("write @%08lx  end data seg\n", proto_bug_get_total_size(pb));

#undef initialization
#undef expression
#undef memory_index
}

void cwasm_section_data_read(struct cwasm_section_data *self,
                             struct proto_bug *pb)
{
#define initialization                                                         \
    do                                                                         \
    {                                                                          \
        uint64_t max = proto_bug_read_varuint(pb, "data::init_size");          \
        cwasm_log("read @%08lx   data seg init size: %" PRIu64 "\n",           \
                  proto_bug_get_total_size(pb), max);                          \
        for (uint64_t i = 0; i < max; i++)                                     \
        {                                                                      \
            cwasm_vector_grow(uint8_t, self->initialization);                  \
            *self->initialization_end++ =                                      \
                proto_bug_read_uint8(pb, "data::init");                        \
        }                                                                      \
    } while (0);

#define expression                                                             \
    cwasm_instruction_expression_read(&self->expression, pb);                  \
    cwasm_log("read @%08lx   data seg expr\n", proto_bug_get_total_size(pb));

#define memory_index                                                           \
    proto_bug_read_uint8(pb, "data::memory_index");                            \
    cwasm_log("read @%08lx   data seg memory index: %d\n",                     \
              proto_bug_get_total_size(pb), self->memory_index);

    self->mode = proto_bug_read_varuint(pb, "data::mode");
    cwasm_log("read @%08lx   begin data seg: mode: %d\n",
              proto_bug_get_total_size(pb), self->mode);
    data_instructions;
    cwasm_log("read @%08lx   end data seg\n", proto_bug_get_total_size(pb));

#undef initialization
#undef expression
#undef memory_index
}
