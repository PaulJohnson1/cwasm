#pragma once

#include <stdint.h>

#include <vector.h>

// note: grammar not proper, memorys should be called memories but macros are
// annoying
struct cwasm_module
{
    cwasm_vector_declare(struct cwasm_section_type, types);
    cwasm_vector_declare(struct cwasm_section_custom, customs);
    cwasm_vector_declare(struct cwasm_section_import, imports);
    cwasm_vector_declare(struct cwasm_section_function, functions);
    cwasm_vector_declare(struct cwasm_section_code, codes);
    cwasm_vector_declare(struct cwasm_section_memory, memorys);
    cwasm_vector_declare(struct cwasm_section_global, globals);
    cwasm_vector_declare(struct cwasm_section_export, exports);
    cwasm_vector_declare(struct cwasm_section_table, tables);
    cwasm_vector_declare(struct cwasm_section_element, elements);
    cwasm_vector_declare(struct cwasm_section_data, datas);
};

extern void cwasm_module_init(struct cwasm_module *);
extern void cwasm_module_free(struct cwasm_module *);
extern void cwasm_module_read(struct cwasm_module *, uint8_t *begin,
                             uint64_t size);
extern void cwasm_module_write(struct cwasm_module *self, uint8_t *begin,
                              uint64_t *size);
