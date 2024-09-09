#include <cwasm.h>

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <pb.h>

#include <consts.h>
#include <section/code.h>
#include <section/export.h>
#include <section/function.h>
#include <section/import.h>
#include <section/type.h>
#include <util.h>

void log_hex(uint8_t *start, uint8_t *end)
{
    while (start != end)
        printf("%02x,", *start++);
    puts("");
}

CWASM_EXPORT
struct cwasm_instruction create_instruction(uint64_t op,
                                            uint64_t immediate_count, ...)
{
    struct cwasm_instruction instruction = {0};
    instruction.op = op;
    va_list args;
    va_start(args, immediate_count);
    for (uint64_t i = 0; i < immediate_count; i++)
    {
        union cwasm_immediate immediate = va_arg(args, union cwasm_immediate);
        cwasm_vector_grow(union cwasm_immediate, instruction.immediates);
        *instruction.immediates_end++ = immediate;
    }

    return instruction;
}

CWASM_EXPORT
void set_instructions(struct cwasm_section_code *code, uint64_t count, ...)
{
    va_list args;
    va_start(args, count);
    for (uint64_t i = 0; i < count; i++)
    {
        struct cwasm_instruction instruction =
            va_arg(args, struct cwasm_instruction);
        cwasm_vector_grow(struct cwasm_instruction,
                          code->expression.instructions);
        *code->expression.instructions_end = instruction;
        code->expression.instructions_end++;
    }
    va_end(args);
}

void read_bytes_from_file(char const *name, uint8_t **data, uint64_t *size)
{
    FILE *file = fopen(name, "rb");
    assert(file);
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *data = malloc(*size);

    if (fread(*data, 1, *size, file) != *size)
        perror("Failed to read file");
    fclose(file);
}

struct cwasm_module read_module_from_file(char const *name)
{
    uint8_t *data;
    uint64_t size;
    read_bytes_from_file(name, &data, &size);
    struct proto_bug pb;
    proto_bug_init(&pb, data);
    struct cwasm_module readed_module;
    cwasm_module_init(&readed_module);
    cwasm_module_read(&readed_module, data, size);

    return readed_module;
}

void save_module_to_file(struct cwasm_module *module, char const *name)
{
    static uint8_t bytes[1024 * 1024 * 128];
    uint64_t size = 0;
    cwasm_module_write(module, bytes, &size);
    FILE *outwasm = fopen(name, "wb");
    size_t written = fwrite(bytes, 1, size, outwasm);
    if (written != size)
        assert(0);
    fclose(outwasm);
}

struct cwasm_module test_create_module()
{
    struct cwasm_module module;
    cwasm_module_init(&module);

    cwasm_vector_grow(struct cwasm_section_import, module.imports);
    cwasm_vector_grow(struct cwasm_section_type, module.types);
    cwasm_vector_grow(struct cwasm_section_function, module.functions);
    cwasm_vector_grow(struct cwasm_section_code, module.codes);
    cwasm_vector_grow(struct cwasm_section_export, module.exports);

    module.imports_end->name = strdup("transform"); // they must be on the heap
    module.imports_end->module = strdup("m");

    module.types_end->signature = malloc(3);
    module.types_end->parameters_size = 1;
    module.types_end->results_size = 1;
    cwasm_section_type_get_parameters(module.types_end)[0] = 127;
    cwasm_section_type_get_results(module.types_end)[0] = 127;

    module.functions_end->type_index = 0;

    module.codes_end->locals = malloc(4);
    module.codes_end->locals_end = module.codes_end->locals_cap =
        module.codes_end->locals + 4;
    module.codes_end->locals[0] = 127;
    module.codes_end->locals[1] = 127;
    module.codes_end->locals[2] = 126;
    module.codes_end->locals[3] = 126;
    module.codes_end->expression.instructions = 0;
    module.codes_end->expression.instructions_end = 0;
    module.codes_end->expression.instructions_cap = 0;
    set_instructions(module.codes_end, 7,
                     create_instruction(cwasm_opcode_i32_const, 1, 123),
                     create_instruction(cwasm_opcode_i32_const, 1, 12),
                     create_instruction(cwasm_opcode_i64_const, 1, 60),
                     create_instruction(cwasm_opcode_drop, 0),
                     create_instruction(cwasm_opcode_i32_add, 0),
                     create_instruction(cwasm_opcode_call, 1, 0),
                     create_instruction(cwasm_opcode_end, 0));

    module.exports_end->name = strdup("lolol");
    module.exports_end->index = 1;
    module.exports_end->type = 0;

    module.imports_end++;
    module.types_end++;
    module.functions_end++;
    module.codes_end++;
    module.exports_end++;

    return module;
}

void test_find_first_diff(char const *a, char const *b)
{
    uint8_t *original_data;
    uint64_t original_size;
    uint8_t *mod_data;
    uint64_t mod_size;

    read_bytes_from_file(a, &original_data, &original_size);
    read_bytes_from_file(b, &mod_data, &mod_size);

    uint64_t min_size = original_size < mod_size ? original_size : mod_size;

    for (uint64_t i = 0; i < min_size; i++)
        if (original_data[i] != mod_data[i])
        {
            fprintf(stderr, "diff at %08lx\n", i);
            exit(1);
        }

    if (min_size != original_size)
        fputs("size is not the same\n", stderr);
}

void test_read_write_sample()
{
    struct cwasm_module module = read_module_from_file("sample.wasm");
    // printf("%lld\n",
    //        module.codes[6].expression.instructions[1].immediates[0].int64);
    save_module_to_file(&module, "sample.mod.wasm");
    cwasm_module_free(&module);

    test_find_first_diff("sample.wasm", "sample.mod.wasm");

    // struct cwasm_module module2 = read_module_from_file("sample.mod.wasm");
    // cwasm_module_free(&module2);
}

void test_read_write_pb()
{
    struct cwasm_module module = test_create_module();
    save_module_to_file(&module, "1.wasm");
    struct cwasm_module module2 = read_module_from_file("1.wasm");
    save_module_to_file(&module2, "2.wasm");

    test_find_first_diff("1.wasm", "2.wasm");

    cwasm_module_free(&module);
    cwasm_module_free(&module2);
}

uint64_t rand_uint64()
{
    uint64_t r = 0;
    for (int i = 0; i < 64; i += 15 /*30*/)
        r = r * ((uint64_t)RAND_MAX + 1) + rand();
    return r;
}

void test_i64_pb()
{
    uint8_t pb_data[1000] = {0};
    struct proto_bug pb;
    proto_bug_init(&pb, pb_data);

    for (uint64_t i = 0; i < 1000 * 1000 * 100; i++)
    {
        // int64_t test = -903075326004949130ll;
        int64_t test = rand_uint64();
        proto_bug_write_varint64(&pb, test, "test");
        proto_bug_reset(&pb);
        int64_t out = proto_bug_read_varint64(&pb, "test");
        if (out != test)
        {
            proto_bug_reset(&pb);
            proto_bug_read_varint64(&pb, "test");
            printf("%ld %ld\n""f6,9e,b8,a0,e9,b8,e8,bb,73\n", test, out);
            log_hex(pb_data, pb_data + proto_bug_get_size(&pb));
            exit(1);
        }
        proto_bug_reset(&pb);
        memset(pb_data, 0, proto_bug_get_size(&pb));
    }
}

CWASM_EXPORT
int main()
{
    // test_read_write_pb();
    // test_read_write_sample();
    // test_find_first_diff();
    test_i64_pb();
}
