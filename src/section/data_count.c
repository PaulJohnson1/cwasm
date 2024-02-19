#include <section/data_count.h>

#include <inttypes.h>

#include <pb.h>

#include <log.h>
#include <consts.h>

void cwasm_section_data_count_free(struct cwasm_section_data_count *self) {}

void cwasm_section_data_count_write(struct cwasm_section_data_count *self,
                                    struct proto_bug *writer)
{
    proto_bug_write_varuint(writer, self->count, "data_count");
    cwasm_log("write    data_count: %" PRIu64 "\n", self->count);
}

void cwasm_section_data_count_read(struct cwasm_section_data_count *self,
                                   struct proto_bug *reader)
{
    self->count = proto_bug_read_varuint(reader, "data_count");
    cwasm_log("read    data_count: %" PRIu64 "\n", self->count);
}
