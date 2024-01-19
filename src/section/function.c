#include <section/function.h>

#include <pb.h>

#include <consts.h>

void cwasm_section_function_free(struct cwasm_section_function *self) {}

void cwasm_section_function_write(struct cwasm_section_function *self,
                                 struct proto_bug *writer)
{
    proto_bug_write_varuint(writer, self->type_index, "function::typeidx");
}

void cwasm_section_function_read(struct cwasm_section_function *self,
                                struct proto_bug *reader)
{
    self->type_index = proto_bug_read_varuint(reader, "function::typeidx");
}
