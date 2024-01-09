#include <section/import.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <pb.h>

#include <consts.h>

void cwasm_section_import_free(struct cwasm_section_import *self)
{
    free(self->module);
    free(self->name);
}

int cwasm_section_import_write(struct cwasm_section_import *self,
                               struct proto_bug *writer)
{
    uint64_t module_size = strlen(self->module);
    uint64_t name_size = strlen(self->name);
    proto_bug_write_varuint(writer, module_size, "import::module::size");
    proto_bug_write_string(writer, self->module, module_size, "import::module");
    proto_bug_write_varuint(writer, name_size, "import::name::size");
    proto_bug_write_string(writer, self->name, name_size, "import::name");
    return cwasm_error_ok;
}

int cwasm_section_import_read(struct cwasm_section_import *self,
                              struct proto_bug *reader)
{
    uint64_t module_size =
        proto_bug_read_varuint(reader, "import::module::size");
    self->module = calloc(module_size + 1, 1);
    proto_bug_read_string(reader, self->module, module_size, "import::module");
    uint64_t name_size = proto_bug_read_varuint(reader, "import::name::size");
    self->name = calloc(name_size + 1, 1);
    proto_bug_read_string(reader, self->name, name_size, "import::name");

    self->type = proto_bug_read_uint8(reader, "import::type");

    switch (self->type)
    {
    case cwasm_external_type_function:
        self->description.table_index = proto_bug_read_varuint(reader, "import::type_index");
        break;
    case cwasm_external_type_table:
        cwasm_type_table_read(&self->description.table, reader);
        break;
    case cwasm_external_type_memory:
        cwasm_type_memory_read(&self->description.memory, reader);
        break;
    case cwasm_external_type_global:
        cwasm_type_global_read(&self->description.global, reader);
        break;
    default:
        assert(0);
    }

    return cwasm_error_ok;
}
