#include <cwasm/section/code.h>

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cwasm/pb.h>

#include <cwasm/consts.h>
#include <cwasm/log.h>

// #define X(op, immediate_count, ...)
#define immediate_opcode_types                                                 \
    X(unreachable, 0)                                                          \
    X(nop, 0)                                                                  \
    X(block, 1, wasm_types_varint32)                                           \
    X(loop, 1, wasm_types_varint32)                                            \
    X(if, 1, wasm_types_varint32)                                              \
    X(else, 0)                                                                 \
    X(end, 0)                                                                  \
    X(br, 1, wasm_types_varuint)                                               \
    X(br_if, 1, wasm_types_varuint)                                            \
    X(return, 0)                                                               \
    X(call, 1, wasm_types_varuint)                                             \
    X(call_indirect, 2, wasm_types_varuint, wasm_types_varuint)                \
    X(drop, 0)                                                                 \
    X(select, 0)                                                               \
                                                                               \
    X(local_get, 1, wasm_types_varuint)                                        \
    X(local_set, 1, wasm_types_varuint)                                        \
    X(local_tee, 1, wasm_types_varuint)                                        \
                                                                               \
    X(global_get, 1, wasm_types_varuint)                                       \
    X(global_set, 1, wasm_types_varuint)                                       \
                                                                               \
    X(i32_load, 2, wasm_types_varuint, wasm_types_varuint)                     \
    X(i64_load, 2, wasm_types_varuint, wasm_types_varuint)                     \
    X(f32_load, 2, wasm_types_varuint, wasm_types_varuint)                     \
    X(f64_load, 2, wasm_types_varuint, wasm_types_varuint)                     \
    X(i32_load8_s, 2, wasm_types_varuint, wasm_types_varuint)                  \
    X(i32_load8_u, 2, wasm_types_varuint, wasm_types_varuint)                  \
    X(i32_load16_s, 2, wasm_types_varuint, wasm_types_varuint)                 \
    X(i32_load16_u, 2, wasm_types_varuint, wasm_types_varuint)                 \
    X(i64_load8_s, 2, wasm_types_varuint, wasm_types_varuint)                  \
    X(i64_load8_u, 2, wasm_types_varuint, wasm_types_varuint)                  \
    X(i64_load16_s, 2, wasm_types_varuint, wasm_types_varuint)                 \
    X(i64_load16_u, 2, wasm_types_varuint, wasm_types_varuint)                 \
    X(i64_load32_s, 2, wasm_types_varuint, wasm_types_varuint)                 \
    X(i64_load32_u, 2, wasm_types_varuint, wasm_types_varuint)                 \
                                                                               \
    X(i32_store, 2, wasm_types_varuint, wasm_types_varuint)                    \
    X(i64_store, 2, wasm_types_varuint, wasm_types_varuint)                    \
    X(f32_store, 2, wasm_types_varuint, wasm_types_varuint)                    \
    X(f64_store, 2, wasm_types_varuint, wasm_types_varuint)                    \
    X(i32_store8, 2, wasm_types_varuint, wasm_types_varuint)                   \
    X(i32_store16, 2, wasm_types_varuint, wasm_types_varuint)                  \
    X(i64_store8, 2, wasm_types_varuint, wasm_types_varuint)                   \
    X(i64_store16, 2, wasm_types_varuint, wasm_types_varuint)                  \
    X(i64_store32, 2, wasm_types_varuint, wasm_types_varuint)                  \
                                                                               \
    X(memory_size, 1, wasm_types_uint8)                                        \
    X(memory_grow, 1, wasm_types_uint8)                                        \
                                                                               \
    X(i32_const, 1, wasm_types_varint32)                                       \
    X(i64_const, 1, wasm_types_varint64)                                       \
    X(f32_const, 1, wasm_types_float32)                                        \
    X(f64_const, 1, wasm_types_float64)                                        \
                                                                               \
    X(i32_eqz, 0)                                                              \
    X(i32_eq, 0)                                                               \
    X(i32_ne, 0)                                                               \
    X(i32_lt_s, 0)                                                             \
    X(i32_lt_u, 0)                                                             \
    X(i32_gt_s, 0)                                                             \
    X(i32_gt_u, 0)                                                             \
    X(i32_le_s, 0)                                                             \
    X(i32_le_u, 0)                                                             \
    X(i32_ge_s, 0)                                                             \
    X(i32_ge_u, 0)                                                             \
                                                                               \
    X(i64_eqz, 0)                                                              \
    X(i64_eq, 0)                                                               \
    X(i64_ne, 0)                                                               \
    X(i64_ls_s, 0)                                                             \
    X(i64_lt_u, 0)                                                             \
    X(i64_gt_s, 0)                                                             \
    X(i64_gt_u, 0)                                                             \
    X(i64_le_s, 0)                                                             \
    X(i64_le_u, 0)                                                             \
    X(i64_ge_s, 0)                                                             \
    X(i64_ge_u, 0)                                                             \
                                                                               \
    X(f32_eq, 0)                                                               \
    X(f32_ne, 0)                                                               \
    X(f32_lt, 0)                                                               \
    X(f32_gt, 0)                                                               \
    X(f32_le, 0)                                                               \
    X(f32_ge, 0)                                                               \
                                                                               \
    X(f64_eq, 0)                                                               \
    X(f64_ne, 0)                                                               \
    X(f64_lt, 0)                                                               \
    X(f64_gt, 0)                                                               \
    X(f64_le, 0)                                                               \
    X(f64_ge, 0)                                                               \
                                                                               \
    X(i32_clz, 0)                                                              \
    X(i32_ctz, 0)                                                              \
    X(i32_popcnt, 0)                                                           \
    X(i32_add, 0)                                                              \
    X(i32_sub, 0)                                                              \
    X(i32_mul, 0)                                                              \
    X(i32_div_s, 0)                                                            \
    X(i32_div_u, 0)                                                            \
    X(i32_rem_s, 0)                                                            \
    X(i32_rem_u, 0)                                                            \
    X(i32_and, 0)                                                              \
    X(i32_or, 0)                                                               \
    X(i32_xor, 0)                                                              \
    X(i32_shl, 0)                                                              \
    X(i32_shr_s, 0)                                                            \
    X(i32_shr_u, 0)                                                            \
    X(i32_rotl, 0)                                                             \
                                                                               \
    X(i64_clz, 0)                                                              \
    X(i64_ctz, 0)                                                              \
    X(i64_popcnt, 0)                                                           \
    X(i64_add, 0)                                                              \
    X(i64_sub, 0)                                                              \
    X(i64_mul, 0)                                                              \
    X(i64_div_s, 0)                                                            \
    X(i64_div_u, 0)                                                            \
    X(i64_rem_s, 0)                                                            \
    X(i64_rem_u, 0)                                                            \
    X(i64_and, 0)                                                              \
    X(i64_or, 0)                                                               \
    X(i64_xor, 0)                                                              \
    X(i64_shl, 0)                                                              \
    X(i64_shr_s, 0)                                                            \
    X(i64_shr_u, 0)                                                            \
    X(i64_rotl, 0)                                                             \
    X(i64_rotr, 0)                                                             \
                                                                               \
    X(f32_abs, 0)                                                              \
    X(f32_neg, 0)                                                              \
    X(f32_ceil, 0)                                                             \
    X(f32_floor, 0)                                                            \
    X(f32_trunc, 0)                                                            \
    X(f32_nearest, 0)                                                          \
    X(f32_sqrt, 0)                                                             \
    X(f32_add, 0)                                                              \
    X(f32_sub, 0)                                                              \
    X(f32_mul, 0)                                                              \
    X(f32_div, 0)                                                              \
    X(f32_min, 0)                                                              \
    X(f32_max, 0)                                                              \
    X(f32_copysign, 0)                                                         \
                                                                               \
    X(f64_abs, 0)                                                              \
    X(f64_neg, 0)                                                              \
    X(f64_ceil, 0)                                                             \
    X(f64_floor, 0)                                                            \
    X(f64_trunc, 0)                                                            \
    X(f64_nearest, 0)                                                          \
    X(f64_sqrt, 0)                                                             \
    X(f64_add, 0)                                                              \
    X(f64_sub, 0)                                                              \
    X(f64_mul, 0)                                                              \
    X(f64_div, 0)                                                              \
    X(f64_min, 0)                                                              \
    X(f64_max, 0)                                                              \
    X(f64_copysign, 0)                                                         \
                                                                               \
    X(i32_wrap_i64, 0)                                                         \
    X(i32_trunc_f32_s, 0)                                                      \
    X(i32_trunc_f32_u, 0)                                                      \
    X(i32_trunc_f64_s, 0)                                                      \
    X(i32_trunc_f64_u, 0)                                                      \
    X(i64_extend_i32_s, 0)                                                     \
    X(i64_extend_i32_u, 0)                                                     \
    X(i64_trunc_f32_s, 0)                                                      \
    X(i64_trunc_f32_u, 0)                                                      \
    X(i64_trunc_f64_s, 0)                                                      \
    X(i64_trunc_f64_u, 0)                                                      \
    X(f32_convert_i32_s, 0)                                                    \
    X(f32_convert_i32_u, 0)                                                    \
    X(f32_convert_i64_s, 0)                                                    \
    X(f32_convert_i64_u, 0)                                                    \
    X(f32_demote_f64, 0)                                                       \
    X(f64_convert_i32_s, 0)                                                    \
    X(f64_convert_i32_u, 0)                                                    \
    X(f64_convert_i64_s, 0)                                                    \
    X(f64_convert_i64_u, 0)                                                    \
    X(f64_promote_f32, 0)                                                      \
    X(i32_reinterpret_f32, 0)                                                  \
    X(i64_reinterpret_f64, 0)                                                  \
    X(f32_reinterpret_f32, 0)                                                  \
    X(f64_reinterpret_f64, 0)                                                  \
    X(ref_null, 1, wasm_types_varuint)

// #define Y(type, union_m_name, ...)
#define immediate_types                                                        \
    Y(varint32, int64)                                                         \
    Y(varint64, int64)                                                         \
    Y(varuint, uint64)                                                         \
    Y(float32, float32)                                                        \
    Y(float64, float64)                                                        \
    Y(uint8, uint64)

enum e_wasm_types
{
#define Y(x, _) wasm_types_##x,
    immediate_types
#undef Y
        wasm_types_max
};

void cwasm_instruction_write(struct cwasm_instruction *self,
                             struct proto_bug *pb)
{
    if (self->op > 0xff)
    {
        proto_bug_write_uint8(pb, self->op >> 8, "instruction::op");
        proto_bug_write_varuint(pb, self->op & 255,
                                "instruction::vu_larger_than_ff");
    }
    else
        proto_bug_write_uint8(pb, self->op, "instruction::op");

    switch (self->op)
    {
#define Y(wasm_type, union_m_name)                                             \
    if (wasm_immediate_types[i] == wasm_types_##wasm_type)                     \
        proto_bug_write_##wasm_type(pb, self->immediates[i].union_m_name,      \
                                    "code::immediate");

#define X(op, immediate_count, ...)                                            \
    case cwasm_opcode_##op:                                                    \
    {                                                                          \
        /* compiler will optimize */                                           \
        enum e_wasm_types wasm_immediate_types[immediate_count] = {            \
            __VA_ARGS__};                                                      \
        for (uint64_t i = 0; i < immediate_count; i++)                         \
        {                                                                      \
            immediate_types                                                    \
        }                                                                      \
        break;                                                                 \
    }
    immediate_opcode_types
#undef X
#undef Y
        case cwasm_opcode_br_table:
    {
        uint64_t immediates_count = self->immediates_end - self->immediates;
        proto_bug_write_varuint(pb, immediates_count - 1, "immediate_br_table");
        for (uint64_t i = 0; i < immediates_count; i++)
            proto_bug_write_varuint(pb, self->immediates[i].uint64,
                                    "immediate");
        break;
    }

    default:
        fprintf(stderr,
                "invalid opcode found while writing: %02" PRId64 "x %" PRId64
                "u\n",
                self->op, self->op);
        assert(0);
        break;
    }
}

void cwasm_instruction_read(struct cwasm_instruction *self,
                            struct proto_bug *pb)
{
    uint32_t op = proto_bug_read_uint8(pb, "instruction::op");
    if (op == 0xfc || op == 0xfd)
        op = (op << 8) |
             proto_bug_read_varuint(pb, "instruction::vu_larger_than_ff");
    self->op = op;
    self->immediates = 0;
    self->immediates_end = 0;
    self->immediates_cap = 0;

    switch (self->op)
    {
#define Y(wasm_type, union_m_name)                                             \
    if (wasm_immediate_types[i] == wasm_types_##wasm_type)                     \
    {                                                                          \
        cwasm_vector_grow(union cwasm_immediate, self->immediates);            \
        (self->immediates_end++)->union_m_name =                               \
            proto_bug_read_##wasm_type(pb, "code::immediate");                 \
    }

#define X(op, immediate_count, ...)                                            \
    case cwasm_opcode_##op:                                                    \
    {                                                                          \
        enum e_wasm_types wasm_immediate_types[immediate_count] = {            \
            __VA_ARGS__};                                                      \
        for (uint64_t i = 0; i < immediate_count; i++)                         \
        {                                                                      \
            immediate_types                                                    \
        }                                                                      \
        break;                                                                 \
    }
    immediate_opcode_types
#undef Y
#undef X
        // instructions with variable number of immediates
        case cwasm_opcode_select_with_type:
    {
        uint64_t max = proto_bug_read_varuint(pb, "element::init_size");
        for (uint64_t i = 0; i < max; i++)
        {
            cwasm_vector_grow(union cwasm_immediate, self->immediates);
            (self->immediates_end++)->uint64 =
                proto_bug_read_varuint(pb, "code::immediate");
        }
        break;
    }
    case cwasm_opcode_br_table:
    {
        uint64_t max = proto_bug_read_varuint(pb, "immediate_br_table");
        for (uint64_t i = 0; i < max + 1; i++)
        {
            cwasm_vector_grow(union cwasm_immediate, self->immediates);
            (self->immediates_end++)->uint64 =
                proto_bug_read_varuint(pb, "code::immediate");
        }

        break;
    }

    default:
        fprintf(stderr,
                "invalid opcode found while reading: %02" PRId64 "x %" PRId64
                "u\n",
                self->op, self->op);
        assert(0);
        break;
    }
}

void cwasm_instruction_expression_write(struct cwasm_instruction_expression *e,
                                        struct proto_bug *pb)
{
    for (struct cwasm_instruction *i = e->instructions; i < e->instructions_end;
         i++)
    {
        cwasm_log("write @%08lx  instr: op: %lu\n",
                  proto_bug_get_total_size(pb), i->op);
        cwasm_instruction_write(i, pb);
    }

    cwasm_log("write @%08lx  end instr expr: size: %" PRIuPTR "\n",
              proto_bug_get_total_size(pb),
              e->instructions_end - e->instructions);
}

void cwasm_instruction_expression_read(struct cwasm_instruction_expression *out,
                                       struct proto_bug *pb)
{
    uint64_t depth = 1;

    while (1)
    {
        cwasm_vector_grow(struct cwasm_instruction, out->instructions);
        cwasm_instruction_read(out->instructions_end, pb);
        uint64_t op = out->instructions_end->op;
        cwasm_log("read @%08lx   instr: op: %lu\n",
                  proto_bug_get_total_size(pb), op);
        out->instructions_end++;
        switch (op)
        {
        case cwasm_opcode_end:
            depth--;
            break;
        case cwasm_opcode_block:
        case cwasm_opcode_loop:
        case cwasm_opcode_if:
            depth++;
            break;
        default:
            break;
        }
        if (depth == 0 && op == cwasm_opcode_end)
            break;
    }

    cwasm_log("read @%08lx   end instr expr: size: %" PRIuPTR "\n",
              proto_bug_get_total_size(pb),
              out->instructions_end - out->instructions);
}

void cwasm_instruction_free(struct cwasm_instruction *i)
{
    free(i->immediates);
}

void cwasm_instruction_expression_free(struct cwasm_instruction_expression *e)
{
    for (struct cwasm_instruction *i = e->instructions; i < e->instructions_end;
         i++)
        cwasm_instruction_free(i);
    free(e->instructions);
}

void cwasm_section_code_free(struct cwasm_section_code *self)
{
    cwasm_instruction_expression_free(&self->expression);
    free(self->locals);
}

void cwasm_section_code_write(struct cwasm_section_code *self,
                              struct proto_bug *pb)
{
    cwasm_log("write @%08lx  begin function body\n",
              proto_bug_get_total_size(pb));
    static uint8_t code_data[1024 * 1024 * 16];
    struct proto_bug code_pb;
    proto_bug_init(&code_pb, code_data);
    code_pb.offset = proto_bug_get_total_size(pb);

    // find amount of local elements
    uint64_t local_amount = 0;
    uint8_t local_type = 0;
    for (uint8_t *i = self->locals; i < self->locals_end; i++)
        if (local_type != *i)
        {
            local_amount++;
            local_type = *i;
        }

    proto_bug_write_varuint(&code_pb, local_amount,
                            "code::local::element_size");
    cwasm_log("write @%08lx  local decl count: %lu\n",
              proto_bug_get_total_size(&code_pb), local_amount);

    local_amount = 0;
    local_type = 0;
    // write locals
    for (uint8_t *i = self->locals; i < self->locals_end; i++)
    {
        if (*i != local_type)
        {
            if (local_amount != 0)
            {
                proto_bug_write_varuint(&code_pb, local_amount,
                                        "code::local::amount");
                proto_bug_write_uint8(&code_pb, local_type,
                                      "code::local::type");

                cwasm_log("write @%08lx   local decl: count: %lu, type: %u\n",
                          proto_bug_get_total_size(&code_pb), local_amount,
                          local_type);
            }
            local_type = *i;
            local_amount = 1;
        }
        else
            local_amount++;
    }

    if (local_amount != 0)
    {
        proto_bug_write_varuint(&code_pb, local_amount, "code::local::amount");
        proto_bug_write_uint8(&code_pb, local_type, "code::local::type");

        cwasm_log("write @%08lx   local decl: count: %lu, type: %u\n",
                  proto_bug_get_total_size(&code_pb), local_amount, local_type);
    }

    cwasm_instruction_expression_write(&self->expression, &code_pb);

    uint64_t byte_count = proto_bug_get_size(&code_pb);
    proto_bug_write_varuint(pb, byte_count, "code::instructions::size");
    proto_bug_write_string_internal(pb, (char *)code_pb.start, byte_count);
}

void cwasm_section_code_read(struct cwasm_section_code *self,
                             struct proto_bug *pb)
{
    cwasm_log("read @%08lx   begin function body\n",
              proto_bug_get_total_size(pb));
    uint8_t *end =
        pb->current + proto_bug_read_varuint(pb, "code::instructions::size");
    uint64_t local_element_count =
        proto_bug_read_varuint(pb, "code::local::element_size");

    cwasm_log("read @%08lx    local decl count: %lu\n",
              proto_bug_get_total_size(pb), local_element_count);

    for (uint64_t i = 0; i < local_element_count; i++)
    {
        uint64_t amount_of_type =
            proto_bug_read_varuint(pb, "code::local::amount");
        uint8_t type = proto_bug_read_uint8(pb, "code::local::type");
        cwasm_log("read @%08lx   local decl: count: %lu, type: %u\n",
                  proto_bug_get_total_size(pb), amount_of_type, type);
        for (uint64_t j = 0; j < amount_of_type; j++)
        {
            cwasm_vector_grow(uint8_t, self->locals);
            *self->locals_end = type;
            self->locals_end++;
        }
    }

    cwasm_instruction_expression_read(&self->expression, pb);
}

void cwasm_section_code_grow_locals(struct cwasm_section_code *self)
{
    cwasm_vector_grow(uint8_t, self->locals);
}

uint8_t *cwasm_section_code_get_locals(struct cwasm_section_code *self)
{
    return self->locals;
}

uint8_t *cwasm_section_code_get_locals_end(struct cwasm_section_code *self)
{
    return self->locals_end;
}

uint8_t *cwasm_section_code_set_locals_end(struct cwasm_section_code *self,
                                           uint8_t *x)
{
    return self->locals_end = x;
}
