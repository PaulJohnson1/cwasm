#include <cwasm/section/table.h>

#include <stdlib.h>
#include <string.h>

#include <cwasm/pb.h>

#include <cwasm/consts.h>
#include <cwasm/log.h>

void cwasm_section_table_free(struct cwasm_section_table *self) {}

void cwasm_section_table_write(struct cwasm_section_table *self,
                               struct proto_bug *pb)
{
    proto_bug_write_varuint(pb, self->type, "table::type");
    uint8_t flags = 0;
    if (self->max != UINT64_MAX)
        flags |= 1;
    proto_bug_write_uint8(pb, flags, "table::flags");
    proto_bug_write_varuint(pb, self->min, "table::min");
    if (flags & 1)
        proto_bug_write_varuint(pb, self->max, "table::max");

    cwasm_log("write @%08lx  type: %lu, min: %lu, max: %lu\n",
              proto_bug_get_total_size(pb), self->type, self->min, self->max);
}

void cwasm_section_table_read(struct cwasm_section_table *self,
                              struct proto_bug *pb)
{
    self->type = proto_bug_read_varuint(pb, "table::type");
    uint8_t flags = proto_bug_read_uint8(pb, "table::flags");
    self->min = proto_bug_read_varuint(pb, "table::min");
    self->max =
        flags & 1 ? proto_bug_read_varuint(pb, "table::max") : UINT64_MAX;

    cwasm_log("read @%08lx   type: %lu, min: %lu, max: %lu\n",
              proto_bug_get_total_size(pb), self->type, self->min, self->max);
}
