#include <cwasm.h>

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <pb.h>

#include <cwasm_consts.h>
#include <section/cwasm_code.h>
#include <section/cwasm_export.h>
#include <section/cwasm_function.h>
#include <section/cwasm_import.h>
#include <section/cwasm_type.h>

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
        if (code->instructions_end >= code->instructions_cap)
        {
            uint64_t capacity = code->instructions_cap - code->instructions;
            // allocate for one extra space since initial space is 0 and 0 * 2 =
            // 0
            struct cwasm_instruction *new_data =
                realloc(code->instructions,
                        (capacity * 2 + 1) * sizeof *code->instructions);
            struct cwasm_instruction *new_data_cap =
                new_data + capacity * 2 + 1;
            code->instructions = new_data;
            code->instructions_end = new_data + capacity;
            code->instructions_cap = new_data_cap;
        }
        *code->instructions_end = instruction;
        code->instructions_end++;
    }
    va_end(args);
}

int main()
{
    struct cwasm_module module;
    cwasm_module_init(&module);

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
}
