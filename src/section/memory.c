#include <section/memory.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_section_memory_free(struct cwasm_section_memory *self) {}

void cwasm_section_memory_write(struct cwasm_section_memory *self,
                                struct proto_bug *pb)
{
    cwasm_type_limit_write(&self->limit, pb);
    cwasm_log("write @%08lx  memory min: %lu, max: %lu\n",
              proto_bug_get_total_size(pb), self->limit.min, self->limit.max);
}

void cwasm_section_memory_read(struct cwasm_section_memory *self,
                               struct proto_bug *pb)
{
    cwasm_type_limit_read(&self->limit, pb);
    cwasm_log("read @%08lx   memory min: %lu, max: %lu\n",
              proto_bug_get_total_size(pb), self->limit.min, self->limit.max);
}
