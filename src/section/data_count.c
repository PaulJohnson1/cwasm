#include <section/data_count.h>

#include <inttypes.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_section_data_count_free(struct cwasm_section_data_count *self) {}

void cwasm_section_data_count_write(struct cwasm_section_data_count *self,
                                    struct proto_bug *pb)
{
    proto_bug_write_varuint(pb, self->count, "data_count");
    cwasm_log("write @%08lx   data_count: %" PRIu64 "\n",
              proto_bug_get_total_size(pb), self->count);
}

void cwasm_section_data_count_read(struct cwasm_section_data_count *self,
                                   struct proto_bug *pb)
{
    self->count = proto_bug_read_varuint(pb, "data_count");
    cwasm_log("read @%08lx   data_count: %" PRIu64 "\n",
              proto_bug_get_total_size(pb), self->count);
}
