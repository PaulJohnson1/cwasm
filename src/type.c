#include <type.h>

#include <assert.h>
#include <inttypes.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_type_limit_write(struct cwasm_type_limit *self,
                            struct proto_bug *writer)
{
    uint8_t flags = 0;
    if (self->max != UINT64_MAX)
        flags |= 1;
    proto_bug_write_uint8(writer, flags, "limit::flags");
    proto_bug_write_varuint(writer, self->min, "limit::min");
    if (flags & 1)
        proto_bug_write_varuint(writer, self->max, "limit::max");
    // cwasm_log("write   limit: flags:%u\tmin:%" PRIu64 "\tmax:%" PRIu64 "\n",
    //           flags, self->min, self->max);
}

void cwasm_type_limit_read(struct cwasm_type_limit *self,
                           struct proto_bug *reader)
{
    uint8_t flags = proto_bug_read_uint8(reader, "limit::flags");
    self->min = proto_bug_read_varuint(reader, "limit::min");
    self->max =
        flags & 1 ? proto_bug_read_varuint(reader, "limit::max") : UINT64_MAX;
    // cwasm_log("read    limit: flags:%u\tmin:%" PRIu64 "\tmax:%" PRIu64 "\n",
    //           flags, self->min, self->max);
}

void cwasm_type_table_write(struct cwasm_type_table *self,
                            struct proto_bug *writer)
{
    proto_bug_write_uint8(writer, self->reference_type,
                          "table::reference_type");
    cwasm_type_limit_write(&self->limit, writer);
    // cwasm_log("write   table: reference_type: %u\n", self->reference_type);
}

void cwasm_type_table_read(struct cwasm_type_table *self,
                           struct proto_bug *reader)
{
    self->reference_type =
        proto_bug_read_uint8(reader, "table::reference_type");
    cwasm_type_limit_read(&self->limit, reader);
    // cwasm_log("read    table: reference_type: %u\n", self->reference_type);
}

void cwasm_type_memory_write(struct cwasm_type_memory *self,
                             struct proto_bug *writer)
{
    cwasm_type_limit_write(&self->limit, writer);
    cwasm_log("write   memory\n");
}

void cwasm_type_memory_read(struct cwasm_type_memory *self,
                            struct proto_bug *reader)
{
    cwasm_type_limit_read(&self->limit, reader);
    cwasm_log("read    memory\n");
}

void cwasm_type_global_write(struct cwasm_type_global *self,
                             struct proto_bug *writer)
{
    proto_bug_write_uint8(writer, self->value_type, "global::value_type");
    proto_bug_write_varuint(writer, self->flags, "global::flags");
    cwasm_log("write   global: value_type: %u\tflags: %" PRIu64 "\n", self->value_type,
              self->flags);
}

void cwasm_type_global_read(struct cwasm_type_global *self,
                            struct proto_bug *reader)
{
    self->value_type = proto_bug_read_uint8(reader, "global::value_type");
    self->flags = proto_bug_read_varuint(reader, "global::flags");

    cwasm_log("read    global: value_type: %u\tflags: %" PRIu64 "\n", self->value_type,
              self->flags);
}
