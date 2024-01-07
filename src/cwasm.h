#pragma once

#include <stdint.h>

// note: grammar not proper, memorys should be called memories but macros are annoying
struct cwasm_module
{
    uint64_t types_size;
    uint64_t imports_size;
    uint64_t functions_size;
    uint64_t codes_size;
    uint64_t memorys_size;
    uint64_t globals_size;
    uint64_t exports_size;
    uint64_t tables_size;
    uint64_t elements_size;
    uint64_t datas_size;
    struct cwasm_section_type *types;
    struct cwasm_section_import *imports;
    struct cwasm_section_function *functions;
    struct cwasm_section_code *codes;
    struct cwasm_section_memory *memorys;
    struct cwasm_section_global *globals;
    struct cwasm_section_export *exports;
    struct cwasm_section_table *tables;
    struct cwasm_section_element *elements;
    struct cwasm_section_data *datas;
};

extern void cwasm_module_init(struct cwasm_module *);
extern void cwasm_module_free(struct cwasm_module *);
extern int cwasm_module_read(struct cwasm_module *, uint8_t *begin, uint64_t size);
extern int cwasm_module_write(struct cwasm_module *self, uint8_t *begin, uint64_t *size);
