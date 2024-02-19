#include <section/type.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <pb.h>

#include <log.h>
#include <consts.h>

void cwasm_section_type_free(struct cwasm_section_type *self)
{
    free(self->signature);
}

int8_t *cwasm_section_type_get_parameters(struct cwasm_section_type *self)
{
    return self->signature;
}

int8_t *cwasm_section_type_get_results(struct cwasm_section_type *self)
{
    return self->signature + self->parameters_size;
}

void cwasm_section_type_write(struct cwasm_section_type *self,
                              struct proto_bug *writer)
{
    proto_bug_write_uint8(writer, 0x60, "mystery byte");
    if (!self->signature)
    {
        proto_bug_write_varuint(writer, 0, "parameters size");
        proto_bug_write_varuint(writer, 0, "results size");
    }

    proto_bug_write_varuint(writer, self->parameters_size, "parameters size");
    proto_bug_write_string(writer, (char *)cwasm_section_type_get_parameters(self),
                           self->parameters_size, "parameters");
    proto_bug_write_varuint(writer, self->results_size, "results size");
    proto_bug_write_string(writer, (char *)cwasm_section_type_get_results(self),
                           self->results_size, "results");
    cwasm_log("write   type seg: param_size: %" PRIu64 "\t results_size: %" PRIu64 "\n",
              self->parameters_size, self->results_size);
}


void cwasm_section_type_read(struct cwasm_section_type *self,
                             struct proto_bug *reader)
{
    if (proto_bug_read_uint8(reader, "mystery byte") != 0x60)
        assert(0);

    self->parameters_size = proto_bug_read_varuint(reader, "parameters size");
    int8_t *parameters_temp = alloca(self->parameters_size);
    proto_bug_read_string(reader, (char *)parameters_temp, self->parameters_size,
                          "parameters");
    self->results_size = proto_bug_read_varuint(reader, "results size");
    self->signature = malloc(self->results_size + self->parameters_size);
    memcpy(cwasm_section_type_get_parameters(self), parameters_temp,
           self->parameters_size);
    proto_bug_read_string(reader, (char *)cwasm_section_type_get_results(self),
                          self->results_size, "results");
    cwasm_log("read    type seg: param_size: %" PRIu64 "\t results_size: %" PRIu64 "\n",
              self->parameters_size, self->results_size);
}