#include <cwasm/section/export.h>

#include <stdlib.h>
#include <string.h>

#include <cwasm/pb.h>

<<<<<<< Updated upstream
#include <consts.h>
=======
#include <cwasm/consts.h>
#include <cwasm/log.h>
>>>>>>> Stashed changes

void cwasm_section_export_free(struct cwasm_section_export *self)
{
    free(self->name);
}

int cwasm_section_export_write(struct cwasm_section_export *self,
                               struct proto_bug *writer)
{
    uint64_t name_size = strlen(self->name);
    proto_bug_write_varuint(writer, name_size, "export::name::size");
    proto_bug_write_string(writer, self->name, name_size, "export::name");
    return cwasm_error_ok;
}

int cwasm_section_export_read(struct cwasm_section_export *self,
                              struct proto_bug *reader)
{
    if (self->name)
        return cwasm_error_no_one_cares;
    uint64_t name_size = proto_bug_read_varuint(reader, "export::name::size");
    self->name = calloc(name_size + 1, sizeof *self->name);
    proto_bug_read_string(reader, self->name, name_size, "export::name");
    return cwasm_error_ok;
}