#include <section/memory.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_section_memory_free(struct cwasm_section_memory *self) {}

void cwasm_section_memory_write(struct cwasm_section_memory *self,
                                struct proto_bug *writer)
{
    cwasm_type_limit_write(&self->limit, writer);
    cwasm_log("write   memory min: %lu, max: %lu\n", self->limit.min,
              self->limit.max);
}

void cwasm_section_memory_read(struct cwasm_section_memory *self,
                               struct proto_bug *reader)
{
    cwasm_type_limit_read(&self->limit, reader);
    cwasm_log("read    memory min: %lu, max: %lu\n", self->limit.min,
              self->limit.max);
}
