#include <cwasm/type.h>

#include <assert.h>

#include <cwasm/pb.h>

#include <cwasm/consts.h>
#include <cwasm/log.h>

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
}

void cwasm_type_limit_read(struct cwasm_type_limit *self,
                           struct proto_bug *reader)
{
    uint8_t flags = proto_bug_read_uint8(reader, "limit::flags");
    self->min = proto_bug_read_varuint(reader, "limit::min");
    self->max =
        flags & 1 ? proto_bug_read_varuint(reader, "limit::max") : UINT64_MAX;
}

void cwasm_type_table_write(struct cwasm_type_table *self,
                            struct proto_bug *writer)
{
    proto_bug_write_uint8(writer, self->reference_type,
                          "table::reference_type");
    cwasm_type_limit_write(&self->limit, writer);
}

void cwasm_type_table_read(struct cwasm_type_table *self,
                           struct proto_bug *reader)
{
    self->reference_type =
        proto_bug_read_uint8(reader, "table::reference_type");
    cwasm_type_limit_read(&self->limit, reader);
}

void cwasm_type_memory_write(struct cwasm_type_memory *self,
                             struct proto_bug *writer)
{
    cwasm_type_limit_write(&self->limit, writer);
}

void cwasm_type_memory_read(struct cwasm_type_memory *self,
                            struct proto_bug *reader)
{
    cwasm_type_limit_read(&self->limit, reader);
}

void cwasm_type_global_write(struct cwasm_type_global *self,
                             struct proto_bug *writer)
{
}

void cwasm_type_global_read(struct cwasm_type_global *self,
                            struct proto_bug *reader)
{
    self->value_type = proto_bug_read_uint8(reader, "global::value_type");
    self->flags = proto_bug_read_varuint(reader, "global::flags");
}

void cwasm_type_description_write(struct cwasm_type_description *self,
                                  struct proto_bug *writer)
{
}

void cwasm_type_description_read(struct cwasm_type_description *self,
                                 struct proto_bug *reader)
{

    self->type = proto_bug_read_uint8(reader, "description::type");

    switch (self->type)
    {
    case cwasm_type_description_type_function:
        self->table_index =
            proto_bug_read_varuint(reader, "description::type_index");
        break;
    case cwasm_type_description_type_table:
        cwasm_type_table_read(&self->table, reader);
        break;
    case cwasm_type_description_type_memory:
        cwasm_type_memory_read(&self->memory, reader);
        break;
    case cwasm_type_description_type_global:
        cwasm_type_global_read(&self->global, reader);
        break;
    default:
        assert(0);
    }
}
