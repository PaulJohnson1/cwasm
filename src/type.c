#include <type.h>

#include <assert.h>
#include <inttypes.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_type_limit_write(struct cwasm_type_limit *self, struct proto_bug *pb)
{
    uint8_t flags = 0;
    if (self->max != UINT64_MAX)
        flags |= 1;
    proto_bug_write_uint8(pb, flags, "limit::flags");
    proto_bug_write_varuint(pb, self->min, "limit::min");
    if (flags & 1)
        proto_bug_write_varuint(pb, self->max, "limit::max");
    // cwasm_log("write @%08lx  limit: flags:%u\tmin:%" PRIu64 "\tmax:%" PRIu64
    // "\n",
    //           proto_bug_get_size(pb), flags, self->min, self->max);
}

void cwasm_type_limit_read(struct cwasm_type_limit *self, struct proto_bug *pb)
{
    uint8_t flags = proto_bug_read_uint8(pb, "limit::flags");
    self->min = proto_bug_read_varuint(pb, "limit::min");
    self->max =
        flags & 1 ? proto_bug_read_varuint(pb, "limit::max") : UINT64_MAX;
    // cwasm_log("read @%08lx   limit: flags:%u\tmin:%" PRIu64 "\tmax:%" PRIu64
    // "\n",
    //           proto_bug_get_size(pb), flags, self->min, self->max);
}

void cwasm_type_table_write(struct cwasm_type_table *self, struct proto_bug *pb)
{
    proto_bug_write_uint8(pb, self->reference_type, "table::reference_type");
    cwasm_type_limit_write(&self->limit, pb);
    // cwasm_log("write @%08lx  table: reference_type: %u\n",
    // proto_bug_get_size(pb), self->reference_type);
}

void cwasm_type_table_read(struct cwasm_type_table *self, struct proto_bug *pb)
{
    self->reference_type = proto_bug_read_uint8(pb, "table::reference_type");
    cwasm_type_limit_read(&self->limit, pb);
    // cwasm_log("read @%08lx   table: reference_type: %u\n",
    // proto_bug_get_size(pb), self->reference_type);
}

void cwasm_type_memory_write(struct cwasm_type_memory *self,
                             struct proto_bug *pb)
{
    cwasm_type_limit_write(&self->limit, pb);
    cwasm_log("write @%08lx  memory\n", proto_bug_get_size(pb));
}

void cwasm_type_memory_read(struct cwasm_type_memory *self,
                            struct proto_bug *pb)
{
    cwasm_type_limit_read(&self->limit, pb);
    cwasm_log("read @%08lx   memory\n", proto_bug_get_size(pb));
}

void cwasm_type_global_write(struct cwasm_type_global *self,
                             struct proto_bug *pb)
{
    proto_bug_write_uint8(pb, self->value_type, "global::value_type");
    proto_bug_write_varuint(pb, self->flags, "global::flags");
    cwasm_log("write @%08lx  global: value_type: %u\tflags: %" PRIu64 "\n",
              proto_bug_get_size(pb), self->value_type, self->flags);
}

void cwasm_type_global_read(struct cwasm_type_global *self,
                            struct proto_bug *pb)
{
    self->value_type = proto_bug_read_uint8(pb, "global::value_type");
    self->flags = proto_bug_read_varuint(pb, "global::flags");

    cwasm_log("read @%08lx   global: value_type: %u\tflags: %" PRIu64 "\n",
              proto_bug_get_size(pb), self->value_type, self->flags);
}
