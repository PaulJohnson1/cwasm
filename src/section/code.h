#pragma once

#include <stdint.h>

struct proto_bug;

union cwasm_immediate
{
    float float32;
    double float64;
    int64_t int64;
    uint64_t uint64;
};

struct cwasm_instruction
{
    uint64_t op;
    union cwasm_immediate *immediates;
    union cwasm_immediate *immediates_end;
    union cwasm_immediate *immediates_cap;
};

extern int cwasm_instruction_write(struct cwasm_instruction *,
                                   struct proto_bug *);
extern int cwasm_instruction_read(struct cwasm_instruction *,
                                  struct proto_bug *);
extern int cwasm_instruction_read_vector(struct proto_bug *,
                                         struct cwasm_instruction **start,
                                         struct cwasm_instruction **end,
                                         struct cwasm_instruction **cap);
extern void cwasm_instruction_free(struct cwasm_instruction *);

struct cwasm_section_code
{
    struct cwasm_instruction *instructions;
    struct cwasm_instruction *instructions_end;
    struct cwasm_instruction *instructions_cap;
    uint8_t *locals;
    uint8_t *locals_end;
    uint8_t *locals_cap;
};

extern void cwasm_section_code_free(struct cwasm_section_code *);
extern int cwasm_section_code_write(struct cwasm_section_code *,
                                    struct proto_bug *);
extern int cwasm_section_code_read(struct cwasm_section_code *,
                                   struct proto_bug *);
