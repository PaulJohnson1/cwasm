#include <section/global.h>

#include <stdlib.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_section_global_free(struct cwasm_section_global *self)
{
    cwasm_instruction_expression_free(&self->expression);
}

void cwasm_section_global_write(struct cwasm_section_global *self,
                               struct proto_bug *writer)
{
    proto_bug_write_varuint(writer, self->type, "global::type");
    cwasm_log("write   begin global: type: %lu\n", self->type);
    cwasm_log("write   begin global init expr\n");
    cwasm_instruction_expression_write(&self->expression, writer);
}

void cwasm_section_global_read(struct cwasm_section_global *self,
                              struct proto_bug *reader)
{
    self->type = proto_bug_read_varuint(reader, "global::type");
    cwasm_log("read    begin global: type: %lu\n", self->type);
    cwasm_log("read    begin global init expr\n");
    cwasm_instruction_expression_read(&self->expression, reader);
}
