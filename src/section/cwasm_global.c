#include <section/cwasm_global.h>

#include <stdlib.h>

#include <pb.h>

#include <cwasm_consts.h>

void cwasm_section_global_free(struct cwasm_section_global *self)
{
    for (struct cwasm_instruction *i = self->instructions; i < self->instructions_end; i++)
        cwasm_instruction_free(i);
    free(self->instructions);
}

int cwasm_section_global_write(struct cwasm_section_global *self, struct proto_bug *writer)
{
    proto_bug_write_varuint(writer, self->type, "global::type");
    for (struct cwasm_instruction *i = self->instructions; i < self->instructions_end; i++)
        cwasm_instruction_write(i, writer);
    return cwasm_error_ok;
}

int cwasm_section_global_read(struct cwasm_section_global *self, struct proto_bug *reader)
{
    self->type = proto_bug_read_varuint(reader, "global::type");
    cwasm_instruction_read_vector(reader, &self->instructions, &self->instructions_end, &self->instructions_cap);
    return cwasm_error_ok;
}
