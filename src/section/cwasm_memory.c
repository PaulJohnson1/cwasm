#include <section/cwasm_memory.h>

#include <pb.h>

#include <cwasm_consts.h>

void cwasm_section_memory_free(struct cwasm_section_memory *self) {}

int cwasm_section_memory_write(struct cwasm_section_memory *self,
                               struct proto_bug *writer)
{
    uint8_t flags = 0;
    if (self->max != UINT64_MAX)
        flags |= 1;
    proto_bug_write_uint8(writer, flags, "memory::flags");
    proto_bug_write_varuint(writer, self->min, "memory::min");
    if (flags & 1)
        proto_bug_write_varuint(writer, self->max, "memory::max");

    return cwasm_error_ok;
}

int cwasm_section_memory_read(struct cwasm_section_memory *self,
                              struct proto_bug *reader)
{
    uint8_t flags = proto_bug_read_uint8(reader, "memory::flags");
    self->min = proto_bug_read_varuint(reader, "memory::min");
    self->max =
        flags & 1 ? proto_bug_read_varuint(reader, "memory::max") : UINT64_MAX;

    return cwasm_error_ok;
}