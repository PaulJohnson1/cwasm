#include <section/import.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_section_import_free(struct cwasm_section_import *self)
{
    free(self->module);
    free(self->name);
}

void cwasm_section_import_write(struct cwasm_section_import *self,
                                struct proto_bug *writer)
{
    uint64_t module_size = strlen(self->module);
    uint64_t name_size = strlen(self->name);
    proto_bug_write_varuint(writer, module_size, "import::module::size");
    proto_bug_write_string(writer, self->module, module_size, "import::module");
    proto_bug_write_varuint(writer, name_size, "import::name::size");
    proto_bug_write_string(writer, self->name, name_size, "import::name");
    // cwasm_type_description_write(&self->description, writer);
}

void cwasm_section_import_read(struct cwasm_section_import *self,
                               struct proto_bug *reader)
{
    uint64_t module_size =
        proto_bug_read_varuint(reader, "import::module::size");
    self->module = calloc(module_size + 1, 1);
    proto_bug_read_string(reader, self->module, module_size, "import::module");
    uint64_t name_size = proto_bug_read_varuint(reader, "import::name::size");
    self->name = calloc(name_size + 1, 1);
    proto_bug_read_string(reader, self->name, name_size, "import::name");

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
        cwasm_type_memory_read(&self->memory, reader);
        break;
    case 3:
        cwasm_type_global_read(&self->global, reader);
        break;
    default:
        assert(0);
    }
    cwasm_log("read    description: type: %u\n", self->type);
    cwasm_log("read    end import seg \"%s\"::\"%s\"\n", self->module,
              self->name);
}
