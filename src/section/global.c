#include <cwasm/section/global.h>

#include <stdlib.h>

#include <cwasm/pb.h>

#include <cwasm/consts.h>
#include <cwasm/log.h>

void cwasm_section_global_free(struct cwasm_section_global *self)
{
    cwasm_instruction_expression_free(&self->expression);
}

void cwasm_section_global_write(struct cwasm_section_global *self,
                                struct proto_bug *pb)
{
    proto_bug_write_varuint(pb, self->type, "global::type");
    cwasm_log("write @%08lx  begin global: type: %lu\n",
              proto_bug_get_total_size(pb), self->type);
    cwasm_log("write @%08lx  begin global init expr\n",
              proto_bug_get_total_size(pb));
    cwasm_instruction_expression_write(&self->expression, pb);
}

void cwasm_section_global_read(struct cwasm_section_global *self,
                               struct proto_bug *pb)
{
    self->type = proto_bug_read_varuint(pb, "global::type");
    cwasm_log("read @%08lx   begin global: type: %lu\n",
              proto_bug_get_total_size(pb), self->type);
    cwasm_log("read @%08lx   begin global init expr\n",
              proto_bug_get_total_size(pb));
    cwasm_instruction_expression_read(&self->expression, pb);
}
