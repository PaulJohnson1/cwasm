#include <cwasm/section/custom.h>

#include <stdlib.h>
#include <string.h>

#include <cwasm/pb.h>

#include <cwasm/consts.h>

void cwasm_section_custom_free(struct cwasm_section_custom *self)
{
    free(self->name);
    free(self->bytes);
}

void cwasm_section_custom_write(struct cwasm_section_custom *self,
                                struct proto_bug *pb)
{
    uint64_t name_size = strlen(self->name);
    proto_bug_write_varuint(pb, name_size, "custom::name::size");
    proto_bug_write_string(pb, self->name, name_size, "custom::name");

    proto_bug_write_varuint(pb, self->bytes_end - self->bytes,
                            "custom::bytes::size");

    for (uint8_t *i = self->bytes; i < self->bytes_end; i++)
        proto_bug_write_uint8(pb, *i, "custom::bytes");
}

void cwasm_section_custom_read(struct cwasm_section_custom *self,
                               struct proto_bug *pb)
{
    uint64_t name_size = proto_bug_read_varuint(pb, "custom::name::size");
    self->name = calloc(name_size + 1, sizeof *self->name);
    proto_bug_read_string(pb, self->name, name_size, "custom::name");

    uint64_t bytes_size = proto_bug_read_varuint(pb, "custom::bytes::size");
    self->bytes = malloc(bytes_size);
    self->bytes_end = self->bytes_cap = self->bytes + bytes_size;
    for (uint8_t *i = self->bytes; i < self->bytes_end; i++)
        *i = proto_bug_read_uint8(pb, "custom::bytes");
}
