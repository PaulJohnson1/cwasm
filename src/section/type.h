#pragma once

#include <stdint.h>

struct proto_bug;

struct cwasm_section_type
{
    uint64_t parameters_size;
    uint64_t results_size;
    // user must not access this directly and should instead use
    // cwasm_section_type_get_parameters OR cwasm_section_type_get_results.
    // this is one variable instead of two so the it is allocated in one block
    // instead of two blocks, which improves cpu cache locality.
    // order goes [p, p, p, p, r, r] assuming there are four parameters and
    // two results
    int8_t *signature;
};

extern int8_t *cwasm_section_type_get_parameters(struct cwasm_section_type *);
extern int8_t *cwasm_section_type_get_results(struct cwasm_section_type *);
extern void cwasm_section_type_free(struct cwasm_section_type *);
extern void cwasm_section_type_read(struct cwasm_section_type *,
                                   struct proto_bug *);
extern void cwasm_section_type_write(struct cwasm_section_type *,
                                    struct proto_bug *);
