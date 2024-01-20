#include <section/export.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
    // cwasm_type_description_write(&self->description, writer);
}

void cwasm_section_export_read(struct cwasm_section_export *self,
                               struct proto_bug *reader)
{
    uint64_t name_size = proto_bug_read_varuint(reader, "export::name::size");
    self->name = calloc(name_size + 1, sizeof *self->name);
    proto_bug_read_string(reader, self->name, name_size, "export::name");

    self->type = proto_bug_read_uint8(reader, "description::type");

    switch (self->type)
    {
    case 0:
        self->table_index =
            proto_bug_read_varuint(reader, "description::type_index");
        break;
    case 1:
        cwasm_type_table_read(&self->table, reader);
        break;
    case 2:
        self->memory_index =
            proto_bug_read_varuint(reader, "description::memory_index");
        break;
    case 3:
        cwasm_type_global_read(&self->global, reader);
        break;
    default:
        assert(0);
    }
    cwasm_log("read    description: type: %u\n", self->type);
    cwasm_log("read    export seg end: name: \"%s\"\n", self->name);
}