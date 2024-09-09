#include <cwasm/section/element.h>

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>

#include <cwasm/pb.h>

#include <cwasm/consts.h>
#include <cwasm/log.h>
#include <cwasm/section/code.h>

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
        funcidx_vector;                                                        \
        break;                                                                 \
    case 1:                                                                    \
        element_kind;                                                          \
        funcidx_vector;                                                        \
        break;                                                                 \
    case 2:                                                                    \
        table_index;                                                           \
        expression;                                                            \
        element_kind;                                                          \
        funcidx_vector;                                                        \
        break;                                                                 \
    case 3:                                                                    \
        element_kind;                                                          \
        funcidx_vector;                                                        \
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
                                 struct proto_bug *pb)
{
#define expression                                                             \
    cwasm_log("write @%08lx  begin elem segment init expr\n",                  \
              proto_bug_get_total_size(pb));                                   \
    cwasm_instruction_expression_write(&self->expression, pb);

#define expression_vector                                                      \
    proto_bug_write_varuint(pb, self->expressions_end - self->expressions,     \
                            "element::expr_vec::count");                       \
    cwasm_log("write @%08lx  elem seg expr vec count: %" PRIuPTR "\n",         \
              proto_bug_get_total_size(pb),                                    \
              self->expressions_end - self->expressions);                      \
    for (struct cwasm_instruction_expression *i = self->expressions;           \
         i < self->expressions_end; i++)                                       \
        cwasm_instruction_expression_write(i, pb);

#define funcidx_vector                                                         \
    cwasm_log("write @%08lx  elem segment type\n",                             \
              proto_bug_get_total_size(pb));                                   \
    proto_bug_write_varuint(pb, self->init_end - self->init,                   \
                            "element::init_size");                             \
    cwasm_log("write @%08lx  elem seg func idx count: %lu\n",                  \
              proto_bug_get_total_size(pb), self->init_end - self->init);      \
    for (uint64_t *i = self->init; i < self->init_end; i++)                    \
    {                                                                          \
        proto_bug_write_varuint(pb, *i, "element::init");                      \
        cwasm_log("write @%08lx  elem seg func idx: %lu\n",                    \
                  proto_bug_get_total_size(pb), *i);                           \
    }

#define table_index                                                            \
    proto_bug_write_varuint(pb, self->table_index, "element::table_index");    \
    cwasm_log("write @%08lx  elem seg table_index: %" PRIu64 "\n",             \
              proto_bug_get_total_size(pb), self->table_index);

#define reference_type                                                         \
    proto_bug_write_varuint(pb, self->type, "element::type");                  \
    cwasm_log("write @%08lx  elem seg type: %d\n",                             \
              proto_bug_get_total_size(pb), self->type);

#define element_kind proto_bug_write_varuint(pb, 0, "element::element_kind");

    proto_bug_write_uint8(pb, self->mode_flags, "element::mode_flags");

    cwasm_log("write @%08lx  begin elem seg: flags: %u\n",
              proto_bug_get_total_size(pb), self->mode_flags);
    element_instructions;
    cwasm_log("write @%08lx  end elem seg\n", proto_bug_get_total_size(pb));

#undef expression
#undef expression_vector
#undef funcidx_vector
#undef table_index
#undef reference_type
#undef element_kind
}

void cwasm_section_element_read(struct cwasm_section_element *self,
                                struct proto_bug *pb)
{
#define expression                                                             \
    cwasm_log("read @%08lx   begin elem segment init expr\n",                  \
              proto_bug_get_total_size(pb));                                   \
    cwasm_instruction_expression_read(&self->expression, pb);

#define expression_vector                                                      \
    do                                                                         \
    {                                                                          \
        uint64_t count = proto_bug_read_varuint(pb, "element::expr_vec::"      \
                                                    "count");                  \
        cwasm_log("read @%08lx   elem seg expr vec count: %" PRIu64 "\n",      \
                  proto_bug_get_total_size(pb), count);                        \
        if (count)                                                             \
        {                                                                      \
            self->expressions = malloc(count * sizeof *self->expressions);     \
            self->expressions_end = self->expressions_cap =                    \
                self->expressions + count;                                     \
                                                                               \
            for (struct cwasm_instruction_expression *i = self->expressions;   \
                 i < self->expressions_end; i++)                               \
                cwasm_instruction_expression_read(i, pb);                      \
        }                                                                      \
    } while (0);

#define funcidx_vector                                                         \
    do                                                                         \
    {                                                                          \
        cwasm_log("read @%08lx   elem segment type\n",                         \
                  proto_bug_get_total_size(pb));                               \
        uint64_t max = proto_bug_read_varuint(pb, "element::init_size");       \
        cwasm_log("read @%08lx   elem seg func idx count: %lu\n",              \
                  proto_bug_get_total_size(pb), max);                          \
        self->init = malloc(max * sizeof *self->init);                         \
        self->init_cap = self->init_end = self->init + max;                    \
        for (uint64_t *i = self->init; i < self->init_end; i++)                \
        {                                                                      \
            *i = proto_bug_read_varuint(pb, "element::init");                  \
            cwasm_log("read @%08lx   elem seg func idx: %lu\n",                \
                      proto_bug_get_total_size(pb), *i);                       \
        }                                                                      \
    } while (0);

#define table_index                                                            \
    self->table_index = proto_bug_read_varuint(pb, "element::table_"           \
                                                   "index");                   \
    cwasm_log("read @%08lx   elem seg table_index: %" PRIu64 "\n",             \
              proto_bug_get_total_size(pb), self->table_index);

#define reference_type                                                         \
    self->type = proto_bug_read_uint8(pb, "element::reference_type");          \
    cwasm_log("read @%08lx   elem seg type: %u\n",                             \
              proto_bug_get_total_size(pb), self->type);

#define element_kind proto_bug_read_varuint(pb, "element::element_kind");

    self->mode_flags = 0b111 & proto_bug_read_uint8(pb, "element::mode_flags");

    cwasm_log("read @%08lx   begin elem seg: flags: %d\n",
              proto_bug_get_total_size(pb), self->mode_flags);
    element_instructions;
    cwasm_log("read @%08lx   end elem seg\n", proto_bug_get_total_size(pb));

#undef expression
#undef expression_vector
#undef funcidx_vector
#undef table_index
#undef reference_type
#undef element_kind
}
