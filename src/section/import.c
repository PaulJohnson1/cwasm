#include <section/import.h>

#include <assert.h>
#include <inttypes.h>
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
                                struct proto_bug *pb)
{
    uint64_t module_size = strlen(self->module);
    uint64_t name_size = strlen(self->name);
    proto_bug_write_varuint(pb, module_size, "import::module::size");
    proto_bug_write_string(pb, self->module, module_size, "import::module");
    proto_bug_write_varuint(pb, name_size, "import::name::size");
    proto_bug_write_string(pb, self->name, name_size, "import::name");
    // cwasm_type_description_write(&self->description, pb);

    proto_bug_write_uint8(pb, self->type, "description::type");

    cwasm_log("write @%08lx  import seg: \"%s\"::\"%s\"\n",
              proto_bug_get_size(pb), self->module, self->name);

    switch (self->type)
    {
    case 0:
        proto_bug_write_varuint(pb, self->table_index,
                                "description::table_index");
        cwasm_log("write @%08lx  description: 0: table_index: %" PRIu64 "\n",
                  proto_bug_get_size(pb), self->table_index);
        break;
    case 1:
        cwasm_type_table_write(&self->table, pb);
        break;
    case 2:
        cwasm_type_memory_write(&self->memory, pb);
        break;
    case 3:
        cwasm_type_global_write(&self->global, pb);
        break;
    default:
        assert(0);
        break;
    }
}

void cwasm_section_import_read(struct cwasm_section_import *self,
                               struct proto_bug *pb)
{
    uint64_t module_size = proto_bug_read_varuint(pb, "import::module::size");
    self->module = calloc(module_size + 1, 1);
    proto_bug_read_string(pb, self->module, module_size, "import::module");
    uint64_t name_size = proto_bug_read_varuint(pb, "import::name::size");
    self->name = calloc(name_size + 1, 1);
    proto_bug_read_string(pb, self->name, name_size, "import::name");

    self->type = proto_bug_read_uint8(pb, "description::type");

    cwasm_log("read @%08lx   import seg: \"%s\"::\"%s\"\n",
              proto_bug_get_size(pb), self->module, self->name);

    switch (self->type)
    {
    case 0:
        self->table_index =
            proto_bug_read_varuint(pb, "description::table_index");
        cwasm_log("read @%08lx   description: 0: table_index: %" PRIu64 "\n",
                  proto_bug_get_size(pb), self->table_index);
        break;
    case 1:
        cwasm_type_table_read(&self->table, pb);
        break;
    case 2:
        cwasm_type_memory_read(&self->memory, pb);
        break;
    case 3:
        cwasm_type_global_read(&self->global, pb);
        break;
    default:
        assert(0);
        break;
    }
}
