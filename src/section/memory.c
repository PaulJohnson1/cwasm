#include <section/memory.h>

#include <pb.h>

#include <consts.h>

void cwasm_section_memory_free(struct cwasm_section_memory *self) {}

int cwasm_section_memory_write(struct cwasm_section_memory *self,
                               struct proto_bug *writer)
{
    cwasm_type_limit_write(&self->limit, writer);
    return cwasm_error_ok;
}

int cwasm_section_memory_read(struct cwasm_section_memory *self,
                              struct proto_bug *reader)
{
    return cwasm_error_ok;
}
