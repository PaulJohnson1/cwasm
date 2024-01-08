#include <type.h>

#include <pb.h>

#include <consts.h>

int type_external_write(struct type_external self, struct proto_bug *writer)
{
    proto_bug_write_uint8(writer, self.type, "external::type");
    proto_bug_write_varuint(writer, self.data, "external::data");

    return cwasm_error_ok;
}

int type_external_read(struct type_external *self, struct proto_bug *reader)
{
    self->type = proto_bug_read_uint8(reader, "external::type");
    self->data = proto_bug_read_varuint(reader, "external::data");

    return cwasm_error_ok;
}
