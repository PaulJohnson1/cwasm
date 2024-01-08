#include <section/table.h>

#include <string.h>
#include <stdlib.h>

#include <pb.h>

#include <consts.h>

void cwasm_section_table_free(struct cwasm_section_table *self)
{
}

int cwasm_section_table_write(struct cwasm_section_table *self, struct proto_bug *writer)
{
    proto_bug_write_varuint(writer, self->type, "table::type");
    uint8_t flags = 0;
    if (self->max != UINT64_MAX)
        flags |= 1;
    proto_bug_write_uint8(writer, flags, "table::flags");
    proto_bug_write_varuint(writer, self->min, "table::min");
    if (flags & 1)
        proto_bug_write_varuint(writer, self->max, "table::max");

    return cwasm_error_ok;
}

int cwasm_section_table_read(struct cwasm_section_table *self, struct proto_bug *reader)
{
    self->type = proto_bug_read_varuint(reader, "table::type");
    uint8_t flags = proto_bug_read_uint8(reader, "table::flags");
    self->min = proto_bug_read_varuint(reader, "table::min");
    self->max = flags & 1 ? proto_bug_read_varuint(reader, "table::max") : UINT64_MAX;

    return cwasm_error_ok;
}