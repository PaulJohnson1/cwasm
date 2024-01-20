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
}

void cwasm_module_write(struct cwasm_module *self, uint8_t *begin,
                        uint64_t *size)
{
    cwasm_log("write begin module\n");
    struct proto_bug writer;
    proto_bug_init(&writer, begin);
    proto_bug_write_uint32(&writer, 0x6d736100, "magic");
    proto_bug_write_uint32(&writer, 1, "version");

    static uint8_t section_data[max_section_size];
#define write_section(name)                                                    \
    do                                                                         \
    {                                                                          \
        if (self->name##s != self->name##s_end)                                \
        {                                                                      \
            proto_bug_write_uint8(&writer, cwasm_const_section_##name,         \
                                  "section id");                               \
            cwasm_log("write   begin section id: %d\n",                        \
                      cwasm_const_section_##name);                             \
            /* must use a separate writer for the section since we must be     \
             * able to get the size and put it before the section data */      \
            struct proto_bug section_writer;                                   \
            proto_bug_init(&section_writer, section_data);                     \
            proto_bug_write_varuint(&section_writer,                           \
                                    self->name##s_end - self->name##s,         \
                                    "element count");                          \
            cwasm_log("write     element count: %lu\n",                        \
                      self->name##s_end - self->name##s);                      \
                                                                               \
            for (struct cwasm_section_##name *i = self->name##s;               \
                 i < self->name##s_end; i++)                                   \
                cwasm_section_##name##_write(i, &section_writer);              \
                                                                               \
            /* copy section writer over to module writer */                    \
            uint64_t byte_count = proto_bug_get_size(&section_writer);         \
            cwasm_log("write   end   section id: %d\tsize: %ld\n",             \
                      cwasm_const_section_##name, byte_count);                 \
            proto_bug_write_varuint(&writer, byte_count, "section data size"); \
            /* want 1:1 copy instead of any debug headers*/                    \
            proto_bug_write_string_internal(                                   \
                &writer, (char *)section_writer.start, byte_count);            \
        }                                                                      \
    } while (0)

    write_section(type);
    write_section(import);
    write_section(function);
    write_section(table);
    write_section(memory);
    write_section(global);
    write_section(export);
    // write_section(start);
    write_section(element);
    write_section(code);
    write_section(data);
    // write_section(data_count);
#undef write_section

    *size = proto_bug_get_size(&writer);
}

void cwasm_module_read(struct cwasm_module *self, uint8_t *begin, uint64_t size)
{
    struct proto_bug reader;
    proto_bug_init(&reader, begin);
    cwasm_log("read  begin module\n");
    if (proto_bug_read_uint32(&reader, "magic") != 0x6d736100)
        assert(0);
    // wasm 2.0 not supported
    if (proto_bug_read_uint32(&reader, "version") != 1)
        assert(0);

    while (proto_bug_get_size(&reader) < size)
    {
        uint8_t section_id = proto_bug_read_uint8(&reader, "section id");
        uint64_t size = proto_bug_read_varuint(&reader, "section data size");
        uint8_t *expected_end = reader.current + size;
        cwasm_log("read    begin section id: %d\tsize: %lu\n", section_id, size);

        switch (section_id)
        {
        case cwasm_const_section_custom:
            cwasm_vector_grow(struct cwasm_section_custom, self->customs);
            cwasm_section_custom_read(self->customs_end++, &reader);
            break;
        case cwasm_const_section_data_count:
            self->data_count = proto_bug_read_varuint(&reader, "data_count");
            break;
#define concat(a, b) a##b
#define read_section(name)                                                     \
    case cwasm_const_section_##name:                                           \
    {                                                                          \
        uint64_t element_count =                                               \
            proto_bug_read_varuint(&reader, "element count");                  \
        cwasm_log("read      element count: %lu\n", element_count);            \
        self->name##s = malloc(element_count * sizeof *self->name##s);         \
        memset(self->name##s, 0, element_count * sizeof *self->name##s);       \
        self->name##s_end = self->name##s_cap = self->name##s + element_count; \
        /*cwasm_vector_set_size(name, s), element_count)*/                     \
        for (struct cwasm_section_##name *i = self->name##s;                   \
             i < self->name##s_end; i++)                                       \
            cwasm_section_##name##_read(i, &reader);                           \
        break;                                                                 \
    }
            read_section(type);
            read_section(import);
            read_section(function);
            read_section(table);
            read_section(memory);
            read_section(global);
            read_section(export);
            //  read_section(start);
            read_section(element);
            read_section(code);
            read_section(data);
//  read_section(data_count);
#undef read_section
        default:
            fprintf(stderr, "reached invalid section id %d @ %" PRIu64 "\n",
                    section_id, proto_bug_get_size(&reader));
            assert(0);
        }
        assert(expected_end == reader.current);

        cwasm_log("read    end   section id: %d\n", section_id);
    }
    cwasm_log("read  end module\n");
}
