#include <cwasm/section/global.h>

#include <stdlib.h>

#include <cwasm/pb.h>

<<<<<<< Updated upstream
#include <consts.h>
=======
#include <cwasm/consts.h>
#include <cwasm/log.h>
>>>>>>> Stashed changes

void cwasm_section_global_free(struct cwasm_section_global *self)
{
    cwasm_instruction_expression_free(&self->expression);
}

int cwasm_section_global_write(struct cwasm_section_global *self,
                               struct proto_bug *writer)
{
    proto_bug_write_varuint(writer, self->type, "global::type");
    cwasm_instruction_expression_write(&self->expression, writer);
    return cwasm_error_ok;
}

int cwasm_section_global_read(struct cwasm_section_global *self,
                              struct proto_bug *reader)
{
    self->type = proto_bug_read_varuint(reader, "global::type");
    cwasm_instruction_expression_read(&self->expression, reader);
    return cwasm_error_ok;
}
