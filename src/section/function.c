#include <section/function.h>

#include <pb.h>

#include <consts.h>
#include <log.h>

void cwasm_section_function_free(struct cwasm_section_function *self) {}

void cwasm_section_function_write(struct cwasm_section_function *self,
                                  struct proto_bug *pb)
{
    proto_bug_write_varuint(pb, self->type_index, "function::typeidx");
    cwasm_log("write @%08lx  function: sig_index: %lu\n",
              proto_bug_get_total_size(pb), self->type_index);
}

void cwasm_section_function_read(struct cwasm_section_function *self,
                                 struct proto_bug *pb)
{
    self->type_index = proto_bug_read_varuint(pb, "function::typeidx");
    cwasm_log("read @%08lx   function: sig_index: %lu\n",
              proto_bug_get_total_size(pb), self->type_index);
}
