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

    cwasm_vector_grow(struct cwasm_section_import, module.imports);
    cwasm_vector_grow(struct cwasm_section_type, module.types);
    cwasm_vector_grow(struct cwasm_section_function, module.functions);
    cwasm_vector_grow(struct cwasm_section_code, module.codes);
    cwasm_vector_grow(struct cwasm_section_export, module.exports);

    module.imports_end[0].name = strdup("transform"); // they must be on the heap
    module.imports_end[0].module = strdup("m");

    module.types_end[0].signature = malloc(3);
    module.types_end[0].parameters_size = 1;
    module.types_end[0].results_size = 1;
    cwasm_section_type_get_parameters(module.types_end)[0] = 127;
    cwasm_section_type_get_results(module.types_end)[0] = 127;

    module.functions_end[0].type_index = 0;

    module.codes_end[0].locals = malloc(1);
    module.codes_end[0].locals_end = module.codes_end[0].locals + 1;
    module.codes_end[0].locals_cap = module.codes_end[0].locals + 1;
    module.codes_end[0].locals[0] = 127;
    module.codes_end[0].expression.instructions = 0;
    module.codes_end[0].expression.instructions_end = 0;
    module.codes_end[0].expression.instructions_cap = 0;
    set_instructions(module.codes_end, 5,
                     create_instruction(cwasm_opcode_i32_const, 1, 123),
                     create_instruction(cwasm_opcode_i32_const, 1, 1234),
                     create_instruction(cwasm_opcode_i32_add, 0),
                     create_instruction(cwasm_opcode_call, 1, 0),
                     create_instruction(cwasm_opcode_end, 0));

    // module.exports_end[0].name = strdup("lolol");
    // module.exports_end[0].type = 1;

    module.imports_end++;
    module.types_end++;
    module.functions_end++;
    module.codes_end++;
    module.exports_end++;

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
    cwasm_module_free(&module);

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
    // FILE *outwasm = fopen("out.wasm", "wb");
    // size_t written = fwrite(new_data, 1, new_size, outwasm);
    // if (written != new_size)
    //     assert(0);
    // fclose(outwasm);

    // fclose(file);
    // free(data);
}
