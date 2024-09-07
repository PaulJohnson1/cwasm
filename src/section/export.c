#include <section/export.h>

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_section_export_free(struct cwasm_section_export *self)
{
    free(self->name);
}

void cwasm_section_export_write(struct cwasm_section_export *self,
                                struct proto_bug *writer)
{
    uint64_t name_size = strlen(self->name);
    proto_bug_write_varuint(writer, name_size, "export::name::size");
    proto_bug_write_string(writer, self->name, name_size, "export::name");
    proto_bug_write_uint8(writer, self->type, "export::type");
    proto_bug_write_varuint(writer, self->index, "export::index");

    // cwasm_log("write   description: type: %u\n", self->type);
    cwasm_log("write   export seg end: type %u, index: %lu, name: \"%s\"\n",
              self->type, self->index, self->name);
}

void cwasm_section_export_read(struct cwasm_section_export *self,
                               struct proto_bug *reader)
{
    uint64_t name_size = proto_bug_read_varuint(reader, "export::name::size");
    self->name = calloc(name_size + 1, sizeof *self->name);
    proto_bug_read_string(reader, self->name, name_size, "export::name");

    self->type = proto_bug_read_uint8(reader, "export::type");
    self->index = proto_bug_read_varuint(reader, "export::index");

    // cwasm_log("read    description: type: %u\n", self->type);
    cwasm_log("read    export seg end: type %u, index: %lu, name: \"%s\"\n",
              self->type, self->index, self->name);
}
