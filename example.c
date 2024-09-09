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

void log_hex(uint8_t *start, uint8_t *end)
{
    while (start != end)
        printf("%d,", *start++);
    puts("");
}

struct cwasm_instruction create_instruction(uint64_t op,
                                            uint64_t immediate_count, ...)
{
    struct cwasm_instruction instruction;
    memset(&instruction, 0, sizeof instruction);
    instruction.op = op;
    va_list args;
    va_start(args, immediate_count);
    for (uint64_t i = 0; i < immediate_count; i++)
    {
        union cwasm_immediate immediate = va_arg(args, union cwasm_immediate);
        if (instruction.immediates_end >= instruction.immediates_cap)
        {
            uint64_t capacity =
                instruction.immediates_cap - instruction.immediates;
            // allocate for one extra space since initial space is 0 and 0 * 2 =
            // 0
            union cwasm_immediate *new_data =
                realloc(instruction.immediates,
                        (capacity * 2 + 1) * sizeof *instruction.immediates);
            union cwasm_immediate *new_data_cap = new_data + capacity * 2 + 1;
            instruction.immediates = new_data;
            instruction.immediates_end = new_data + capacity;
            instruction.immediates_cap = new_data_cap;
        }
        *instruction.immediates_end = immediate;
        instruction.immediates_end++;
    }

    return instruction;
}

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

int main()
{
    struct cwasm_module module;
    cwasm_module_init(&module);

<<<<<<< Updated upstream:src/test-main.c
    // module.imports_size = 1;
    // module.types_size = 1;
    // module.functions_size = 1;
    // module.codes_size = 1;
    // module.exports_size = 1;
    // module.imports = malloc(module.imports_size * sizeof *module.imports);
    // module.types = malloc(module.types_size * sizeof *module.types);
    // module.functions = malloc(module.functions_size * sizeof
    // *module.functions); module.codes = malloc(module.codes_size * sizeof
    // *module.codes); module.exports = malloc(module.exports_size * sizeof
    // *module.exports);

    // module.imports[0].name = strdup("transform"); // they must be on the heap
    // module.imports[0].module = strdup("m");

    // module.types[0].signature = malloc(3);
    // module.types[0].parameters_size = 1;
    // module.types[0].results_size = 1;
    // cwasm_section_type_get_parameters(module.types)[0] = 127;
    // cwasm_section_type_get_results(module.types)[0] = 127;

    // module.functions[0].type_index = 0;

    // module.codes[0].locals = malloc(1);
    // module.codes[0].locals_end = module.codes[0].locals + 1;
    // module.codes[0].locals_cap = module.codes[0].locals + 1;
    // module.codes[0].locals[0] = 127;
    // module.codes[0].instructions = 0;
    // module.codes[0].instructions_end = 0;
    // module.codes[0].instructions_cap = 0;
    // set_instructions(module.codes, 5,
    //     create_instruction(cwasm_opcode_i32_const, 1, 123),
    //     create_instruction(cwasm_opcode_i32_const, 1, 1234),
    //     create_instruction(cwasm_opcode_i32_add, 0),
    //     create_instruction(cwasm_opcode_call, 1, 0),
    //     create_instruction(cwasm_opcode_end, 0)
    // );

    // module.exports[0].name = strdup("lolol");
    // module.exports[0].type.data = 1;
    // module.exports[0].type.type = 0;

    // static uint8_t data[1024 * 1024];
    // uint64_t size;
    // int err = cwasm_module_write(&module, data, &size);
    // assert(!err);
    // log_hex(data, data + size);

    // FILE *file = fopen("sample.wasm", "rb");
    // // Seek to the end of the file to determine its size
    // fseek(file, 0, SEEK_END);
    // size_t size = ftell(file);
    // fseek(file, 0, SEEK_SET); // Seek back to the start of the file

    // // Allocate memory to hold the file data
    // unsigned char *data = malloc(size);

    // // Read the file into memory
    // if (fread(data, 1, size, file) != size)
    // {
    //     perror("Failed to read file");
    // }
    // // Close the file as it is no longer needed
    // fclose(file);

    // struct proto_bug reader;
    // proto_bug_init(&reader, data);
    // struct cwasm_module readed_module;
    // cwasm_module_init(&readed_module);
    // int err = cwasm_module_read(&readed_module, data, size);
    // // log_hex(data, data + size);
    // printf("%d\n", err);

    // cwasm_module_free(&readed_module);
    // cwasm_module_free(&module);

    // return 0;

    FILE *file = fopen("sample.wasm", "rb");
    fseek(file, 0, SEEK_END);
    uint64_t file_length = ftell(file);
    uint8_t *data = malloc(file_length * 2);
    rewind(file);
    (void)fread(data, file_length, 1, file);
    cwasm_module_read(&module, data, file_length);
    static uint8_t
        new_data[100000000]; // really large for when you do protobug builds
    uint64_t new_size;
    cwasm_module_write(&module, new_data, &new_size);
    cwasm_module_free(&module);
    FILE *outwasm = fopen("out.wasm", "wb");
    size_t written = fwrite(new_data, 1, new_size, outwasm);
    if (written != new_size)
        assert(0);
    fclose(outwasm);

    fclose(file);
    free(data);
=======
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

    if (mod_size != original_size)
        fputs("size is not the same\n", stderr);

    puts("they are the same");
}

void test_read_write_sample()
{
    struct cwasm_module module = read_module_from_file("sample.wasm");    //        module.codes[6].expression.instructions[1].immediates[0].int64);
    save_module_to_file(&module, "sample.mod.wasm");
    cwasm_module_free(&module);

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

void test_save_module()
{
    struct cwasm_module module = test_create_module();
    save_module_to_file(&module, "out.wasm");
}

CWASM_EXPORT
int main()
{
    test_save_module();
    // test_read_write_pb();
    // test_read_write_sample();
    // test_find_first_diff("sample.wasm", "sample.mod.wasm");
    // test_i64_pb();
>>>>>>> Stashed changes:example.c
}
