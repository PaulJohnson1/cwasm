#include <type.h>

#include <pb.h>

#include <consts.h>

int cwasm_cwasm_type_external_write(struct cwasm_type_external self,
                                    struct proto_bug *writer)
{
    proto_bug_write_uint8(writer, self.type, "external::type");
    proto_bug_write_varuint(writer, self.data, "external::data");

    return cwasm_error_ok;
}

int cwasm_cwasm_type_external_read(struct cwasm_type_external *self,
                                   struct proto_bug *reader)
{
    self->type = proto_bug_read_uint8(reader, "external::type");
    self->data = proto_bug_read_varuint(reader, "external::data");

    return cwasm_error_ok;
}
