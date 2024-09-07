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
                                struct proto_bug *pb)
{
    uint64_t name_size = strlen(self->name);
    proto_bug_write_varuint(pb, name_size, "export::name::size");
    proto_bug_write_string(pb, self->name, name_size, "export::name");
    proto_bug_write_uint8(pb, self->type, "export::type");
    proto_bug_write_varuint(pb, self->index, "export::index");

    // cwasm_log("write @%08lx  description: type: %u\n",
    // proto_bug_get_size(pb),self->type);
    cwasm_log(
        "write @%08lx  export seg end: type %u, index: %lu, name: \"%s\"\n",
        proto_bug_get_size(pb), self->type, self->index, self->name);
}

void cwasm_section_export_read(struct cwasm_section_export *self,
                               struct proto_bug *pb)
{
    uint64_t name_size = proto_bug_read_varuint(pb, "export::name::size");
    self->name = calloc(name_size + 1, sizeof *self->name);
    proto_bug_read_string(pb, self->name, name_size, "export::name");

    self->type = proto_bug_read_uint8(pb, "export::type");
    self->index = proto_bug_read_varuint(pb, "export::index");

    // cwasm_log("read @%08lx   description: type: %u\n",
    // proto_bug_get_size(pb),self->type);
    cwasm_log(
        "read @%08lx   export seg end: type %u, index: %lu, name: \"%s\"\n",
        proto_bug_get_size(pb), self->type, self->index, self->name);
}
