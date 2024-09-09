#include <cwasm.h>

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pb.h>

#include <consts.h>
#include <log.h>
#include <section/code.h>
#include <section/custom.h>
#include <section/data.h>
#include <section/data_count.h>
#include <section/element.h>
#include <section/export.h>
#include <section/function.h>
#include <section/global.h>
#include <section/import.h>
#include <section/memory.h>
#include <section/table.h>
#include <section/type.h>

#define max_section_size (1024 * 1024 * 8)

void cwasm_module_init(struct cwasm_module *self)
{
    memset(self, 0, sizeof *self);
}

struct cwasm_module *cwasm_module_new()
{
    return calloc(1, sizeof(struct cwasm_module));
}

void cwasm_module_free(struct cwasm_module *self)
{
#define free_section(name)                                                     \
    {                                                                          \
                                                                               \
        for (struct cwasm_section_##name *i = self->name##s;                   \
             i < self->name##s_end; i++)                                       \
            cwasm_section_##name##_free(i);                                    \
        free(self->name##s);                                                   \
    }
    free_section(custom);
    free_section(code);
    free_section(data);
    free_section(element);
    free_section(export);
    free_section(function);
    free_section(global);
    free_section(import);
    free_section(memory);
    free_section(table);
    free_section(type);
    free(self->data_count);
}

#define x(section)                                                             \
    struct cwasm_section_##section *cwasm_module_get_##section##s(             \
        struct cwasm_module *self)                                             \
    {                                                                          \
        return self->section##s;                                               \
    }                                                                          \
    uint64_t cwasm_##section##_get_byte_size()                                 \
    {                                                                          \
        return sizeof(struct cwasm_section_##section);                         \
    }                                                                          \
    uint64_t cwasm_module_get_##section##_size(struct cwasm_module *self)      \
    {                                                                          \
        return self->section##s_end - self->section##s;                        \
    }                                                                          \
    void cwasm_module_grow_##section(struct cwasm_module *self)                \
    {                                                                          \
        cwasm_vector_grow(struct cwasm_section_##section, self->section##s);   \
    }

cwasm_sections(x)
#undef x

    void cwasm_module_write(struct cwasm_module *self, uint8_t *begin,
                            uint64_t *size)
{
    cwasm_log("write @%08x  begin module\n", 0);
    struct proto_bug pb;
    proto_bug_init(&pb, begin);
    proto_bug_write_uint32(&pb, 0x6d736100, "magic");
    proto_bug_write_uint32(&pb, 1, "version");

    static uint8_t section_data[max_section_size];
#define write_section(name)                                                    \
    do                                                                         \
    {                                                                          \
        if (self->name##s != self->name##s_end)                                \
        {                                                                      \
            proto_bug_write_uint8(&pb, cwasm_const_section_##name,             \
                                  "section id");                               \
            cwasm_log("write @%08lx  begin section id: %d\n",                  \
                      proto_bug_get_total_size(&pb),                           \
                      cwasm_const_section_##name);                             \
            /* must use a separate pb for the section since we must be         \
             * able to get the size and put it before the section data */      \
            struct proto_bug section_pb;                                       \
            proto_bug_init(&section_pb, section_data);                         \
            section_pb.offset = proto_bug_get_size(&pb);                       \
            proto_bug_write_varuint(&section_pb,                               \
                                    self->name##s_end - self->name##s,         \
                                    "element count");                          \
            cwasm_log("write @%08lx  element count: %" PRIuPTR "\n",           \
                      proto_bug_get_total_size(&section_pb),                   \
                      self->name##s_end - self->name##s);                      \
                                                                               \
            for (struct cwasm_section_##name *i = self->name##s;               \
                 i < self->name##s_end; i++)                                   \
                cwasm_section_##name##_write(i, &section_pb);                  \
                                                                               \
            /* copy section pb over to module pb */                            \
            uint64_t byte_count = proto_bug_get_size(&section_pb);             \
            cwasm_log("write @%08lx  end   section id: %d\tsize: %" PRIu64     \
                      "\n",                                                    \
                      proto_bug_get_total_size(&section_pb),                   \
                      cwasm_const_section_##name, byte_count);                 \
            proto_bug_write_varuint(&pb, byte_count, "section data size");     \
            /* want 1:1 copy instead of any debug headers*/                    \
            proto_bug_write_string_internal(&pb, (char *)section_pb.start,     \
                                            byte_count);                       \
        }                                                                      \
    } while (0)

    write_section(type);
    write_section(import);
    write_section(function);
    write_section(table);
    write_section(memory);
    write_section(global);
    write_section(export);
    write_section(element);
    write_section(code);
    write_section(data);
    if (self->data_count)
    {

        cwasm_log("write  @%08lx begin section id: %d\n",
                  proto_bug_get_size(&pb), cwasm_const_section_data_count);
        struct proto_bug section_pb;
        proto_bug_init(&section_pb, section_data);
        section_pb.offset = proto_bug_get_size(&pb);
        cwasm_section_data_count_write(self->data_count, &section_pb);
        uint64_t byte_count = proto_bug_get_size(&section_pb);
        proto_bug_write_varuint(&pb, byte_count, "section data size");
        proto_bug_write_string_internal(&pb, (char *)section_pb.start,
                                        byte_count);
        cwasm_log("write  @%08lx end section id: %d\tsize: %" PRIu64 "\n",
                  proto_bug_get_total_size(&pb), cwasm_const_section_data_count,
                  byte_count);
    }

#undef write_section

    cwasm_log("write  @%08lx end module\n", proto_bug_get_total_size(&pb));
    *size = proto_bug_get_total_size(&pb);
}

void cwasm_module_read(struct cwasm_module *self, uint8_t *begin, uint64_t size)
{
    struct proto_bug pb;
    proto_bug_init(&pb, begin);
    cwasm_log("read @%08x  begin module\n", 0);
    if (proto_bug_read_uint32(&pb, "magic") != 0x6d736100)
        assert(0);
    // wasm 2.0 not supported
    if (proto_bug_read_uint32(&pb, "version") != 1)
        assert(0);

    while (proto_bug_get_total_size(&pb) < size)
    {
        uint8_t section_id = proto_bug_read_uint8(&pb, "section id");
        uint64_t size = proto_bug_read_varuint(&pb, "section data size");
        uint8_t *expected_end = pb.current + size;
        cwasm_log("read @%08lx section id: %d\tsize: %" PRId64 "u\n",
                  proto_bug_get_total_size(&pb), section_id, size);

        switch (section_id)
        {
        case cwasm_const_section_custom:
            cwasm_vector_grow(struct cwasm_section_custom, self->customs);
            cwasm_section_custom_read(self->customs_end++, &pb);
            break;
        case cwasm_const_section_data_count:
        {
            self->data_count = malloc(sizeof *self->data_count);
            cwasm_section_data_count_read(self->data_count, &pb);
            break;
        }
#define read_section(name)                                                     \
    case cwasm_const_section_##name:                                           \
    {                                                                          \
        uint64_t element_count = proto_bug_read_varuint(&pb, "element count"); \
        cwasm_log("read @%08lx   element count: %" PRIu64 "u\n",               \
                  proto_bug_get_total_size(&pb), element_count);               \
        self->name##s = malloc(element_count * sizeof *self->name##s);         \
        memset(self->name##s, 0, element_count * sizeof *self->name##s);       \
        self->name##s_end = self->name##s_cap = self->name##s + element_count; \
        for (struct cwasm_section_##name *i = self->name##s;                   \
             i < self->name##s_end; i++)                                       \
            cwasm_section_##name##_read(i, &pb);                               \
        break;                                                                 \
    }
            read_section(type);
            read_section(import);
            read_section(function);
            read_section(table);
            read_section(memory);
            read_section(global);
            read_section(export);
            read_section(element);
            read_section(code);
            read_section(data);
#undef read_section
        default:
            fprintf(stderr, "reached invalid section id %d @ %" PRIu64 "\n",
                    section_id, proto_bug_get_total_size(&pb));
            assert(0);
        }
        assert(expected_end == pb.current);

        cwasm_log("read @%08lx end   section id: %d\n",
                  proto_bug_get_total_size(&pb), section_id);
    }
    cwasm_log("read @%08lx end module\n", proto_bug_get_total_size(&pb));
}
