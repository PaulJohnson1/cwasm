#include <section/type.h>

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

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
                              struct proto_bug *pb)
{
    proto_bug_write_uint8(pb, 0x60, "mystery byte");
    if (!self->signature)
    {
        proto_bug_write_varuint(pb, 0, "parameters size");
        proto_bug_write_varuint(pb, 0, "results size");
    }

    proto_bug_write_varuint(pb, self->parameters_size, "parameters size");
    proto_bug_write_string(pb, (char *)cwasm_section_type_get_parameters(self),
                           self->parameters_size, "parameters");
    proto_bug_write_varuint(pb, self->results_size, "results size");
    proto_bug_write_string(pb, (char *)cwasm_section_type_get_results(self),
                           self->results_size, "results");
    cwasm_log("write @%08lx  type seg: param_size: %" PRIu64
              "\t results_size: %" PRIu64 "\n",
              proto_bug_get_size(pb), self->parameters_size,
              self->results_size);
}

void cwasm_section_type_read(struct cwasm_section_type *self,
                             struct proto_bug *pb)
{
    if (proto_bug_read_uint8(pb, "mystery byte") != 0x60)
        assert(0);

    self->parameters_size = proto_bug_read_varuint(pb, "parameters size");
    int8_t *parameters_temp = alloca(self->parameters_size);
    proto_bug_read_string(pb, (char *)parameters_temp, self->parameters_size,
                          "parameters");
    self->results_size = proto_bug_read_varuint(pb, "results size");
    self->signature = malloc(self->results_size + self->parameters_size);
    memcpy(cwasm_section_type_get_parameters(self), parameters_temp,
           self->parameters_size);
    proto_bug_read_string(pb, (char *)cwasm_section_type_get_results(self),
                          self->results_size, "results");
    cwasm_log("read @%08lx   type seg: param_size: %" PRIu64
              "\t results_size: %" PRIu64 "\n",
              proto_bug_get_size(pb), self->parameters_size,
              self->results_size);
}