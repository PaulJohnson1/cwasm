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
        printf("%d,", *start++);
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
    struct proto_bug reader;
    proto_bug_init(&reader, data);
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

void test_create_module()
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

    module.codes_end->locals = malloc(1);
    module.codes_end->locals_end = module.codes_end->locals_cap =
        module.codes_end->locals + 1;
    module.codes_end->locals[0] = 127;
    module.codes_end->expression.instructions = 0;
    module.codes_end->expression.instructions_end = 0;
    module.codes_end->expression.instructions_cap = 0;
    set_instructions(module.codes_end, 5,
                     create_instruction(cwasm_opcode_i32_const, 1, 123),
                     create_instruction(cwasm_opcode_i32_const, 1, 1234),
                     create_instruction(cwasm_opcode_i32_add, 0),
                     create_instruction(cwasm_opcode_call, 1, 0),
                     create_instruction(cwasm_opcode_end, 0));

    // module.exports_end->name = strdup("lolol");
    // module.exports_end->type = 1;

    module.imports_end++;
    module.types_end++;
    module.functions_end++;
    module.codes_end++;
    module.exports_end++;

    cwasm_module_free(&module);
}

void test_read_write_sample()
{
    struct cwasm_module module = read_module_from_file("sample.wasm");
    save_module_to_file(&module, "sample.mod.wasm");
    cwasm_module_free(&module);
}

void test_find_first_diff()
{
    uint8_t *original_data;
    uint64_t original_size;
    uint8_t *mod_data;
    uint64_t mod_size;

    read_bytes_from_file("sample.wasm", &original_data, &original_size);
    read_bytes_from_file("sample.mod.wasm", &mod_data, &mod_size);

    uint64_t min_size = original_size < mod_size ? original_size : mod_size;

    for (uint64_t i = 0; i < mod_size; i++)
    {
        if (original_data[i] != mod_data[i])
        {
            fprintf(stderr, "diff at %08lx\n", i);
            exit(1);
        }
    }
}

CWASM_EXPORT
int main()
{
    // test_create_module();

    // static uint8_t data[1024 * 1024];
    // uint64_t size;
    // int err = cwasm_module_write(&module, data, &size);
    // assert(!err);
    // log_hex(data, data + size);

    test_read_write_sample();
    test_find_first_diff();

    return 0;

    // FILE *file = fopen("sample.wasm", "rb");
    // fseek(file, 0, SEEK_END);
    // uint64_t file_length = ftell(file);
    // uint8_t *data = malloc(file_length * 2);
    // rewind(file);
    // (void)fread(data, file_length, 1, file);
    // cwasm_module_read(&module, data, file_length);
    // static uint8_t
    //     new_data[100000000]; // really large for when you do protobug builds
    // uint64_t new_size;
    // cwasm_module_write(&module, new_data, &new_size);
    // cwasm_module_free(&module);

    // fclose(file);
    // free(data);
}
