#pragma once

// note: does not include custom, start or data_count
#define cwasm_sections(X)                                                      \
    X(type)                                                                    \
    X(import)                                                                  \
    X(function)                                                                \
    X(table)                                                                   \
    X(memory)                                                                  \
    X(global)                                                                  \
    X(export)                                                                  \
    X(element)                                                                 \
    X(code)                                                                    \
    X(data)

enum cwasm_const_section
{
    cwasm_const_section_custom,
    cwasm_const_section_type,
    cwasm_const_section_import,
    cwasm_const_section_function,
    cwasm_const_section_table,
    cwasm_const_section_memory,
    cwasm_const_section_global,
    cwasm_const_section_export,
    cwasm_const_section_start,
    cwasm_const_section_element,
    cwasm_const_section_code,
    cwasm_const_section_data,
    cwasm_const_section_data_count
};

enum cwasm_reference_type
{
    cwasm_reference_type_function_reference = -16,
    cwasm_reference_type_external_reference = -17
};

enum cwasm_ocode
{
    cwasm_opcode_unreachable = 0,
    cwasm_opcode_nop = 1,
    cwasm_opcode_block = 2,
    cwasm_opcode_loop = 3,
    cwasm_opcode_if = 4,
    cwasm_opcode_else = 5,
    cwasm_opcode_end = 11,
    cwasm_opcode_br = 12,
    cwasm_opcode_br_if = 13,
    cwasm_opcode_br_table = 14,
    cwasm_opcode_return = 15,
    cwasm_opcode_call = 16,
    cwasm_opcode_call_indirect = 17,
    cwasm_opcode_drop = 26,
    cwasm_opcode_select = 27,
    cwasm_opcode_select_with_type = 28,

    cwasm_opcode_local_get = 32,
    cwasm_opcode_local_set = 33,
    cwasm_opcode_local_tee = 34,

    cwasm_opcode_global_get = 35,
    cwasm_opcode_global_set = 36,

    cwasm_opcode_i32_load = 40,
    cwasm_opcode_i64_load = 41,
    cwasm_opcode_f32_load = 42,
    cwasm_opcode_f64_load = 43,
    cwasm_opcode_i32_load8_s = 44,
    cwasm_opcode_i32_load8_u = 45,
    cwasm_opcode_i32_load16_s = 46,
    cwasm_opcode_i32_load16_u = 47,
    cwasm_opcode_i64_load8_s = 48,
    cwasm_opcode_i64_load8_u = 49,
    cwasm_opcode_i64_load16_s = 50,
    cwasm_opcode_i64_load16_u = 51,
    cwasm_opcode_i64_load32_s = 52,
    cwasm_opcode_i64_load32_u = 53,

    cwasm_opcode_i32_store = 54,
    cwasm_opcode_i64_store = 55,
    cwasm_opcode_f32_store = 56,
    cwasm_opcode_f64_store = 57,
    cwasm_opcode_i32_store8 = 58,
    cwasm_opcode_i32_store16 = 59,
    cwasm_opcode_i64_store8 = 60,
    cwasm_opcode_i64_store16 = 61,
    cwasm_opcode_i64_store32 = 62,

    cwasm_opcode_memory_size = 63,
    cwasm_opcode_memory_grow = 64,

    cwasm_opcode_i32_const = 65,
    cwasm_opcode_i64_const = 66,
    cwasm_opcode_f32_const = 67,
    cwasm_opcode_f64_const = 68,

    cwasm_opcode_i32_eqz = 69,
    cwasm_opcode_i32_eq = 70,
    cwasm_opcode_i32_ne = 71,
    cwasm_opcode_i32_lt_s = 72,
    cwasm_opcode_i32_lt_u = 73,
    cwasm_opcode_i32_gt_s = 74,
    cwasm_opcode_i32_gt_u = 75,
    cwasm_opcode_i32_le_s = 76,
    cwasm_opcode_i32_le_u = 77,
    cwasm_opcode_i32_ge_s = 78,
    cwasm_opcode_i32_ge_u = 79,

    cwasm_opcode_i64_eqz = 80,
    cwasm_opcode_i64_eq = 81,
    cwasm_opcode_i64_ne = 82,
    cwasm_opcode_i64_ls_s = 83,
    cwasm_opcode_i64_lt_u = 84,
    cwasm_opcode_i64_gt_s = 85,
    cwasm_opcode_i64_gt_u = 86,
    cwasm_opcode_i64_le_s = 87,
    cwasm_opcode_i64_le_u = 88,
    cwasm_opcode_i64_ge_s = 89,
    cwasm_opcode_i64_ge_u = 90,

    cwasm_opcode_f32_eq = 91,
    cwasm_opcode_f32_ne = 92,
    cwasm_opcode_f32_lt = 93,
    cwasm_opcode_f32_gt = 94,
    cwasm_opcode_f32_le = 95,
    cwasm_opcode_f32_ge = 96,

    cwasm_opcode_f64_eq = 97,
    cwasm_opcode_f64_ne = 98,
    cwasm_opcode_f64_lt = 99,
    cwasm_opcode_f64_gt = 100,
    cwasm_opcode_f64_le = 101,
    cwasm_opcode_f64_ge = 102,

    cwasm_opcode_i32_clz = 103,
    cwasm_opcode_i32_ctz = 104,
    cwasm_opcode_i32_popcnt = 105,
    cwasm_opcode_i32_add = 106,
    cwasm_opcode_i32_sub = 107,
    cwasm_opcode_i32_mul = 108,
    cwasm_opcode_i32_div_s = 109,
    cwasm_opcode_i32_div_u = 110,
    cwasm_opcode_i32_rem_s = 111,
    cwasm_opcode_i32_rem_u = 112,
    cwasm_opcode_i32_and = 113,
    cwasm_opcode_i32_or = 114,
    cwasm_opcode_i32_xor = 115,
    cwasm_opcode_i32_shl = 116,
    cwasm_opcode_i32_shr_s = 117,
    cwasm_opcode_i32_shr_u = 118,
    cwasm_opcode_i32_rotl = 119,

    cwasm_opcode_i64_clz = 121,
    cwasm_opcode_i64_ctz = 122,
    cwasm_opcode_i64_popcnt = 123,
    cwasm_opcode_i64_add = 124,
    cwasm_opcode_i64_sub = 125,
    cwasm_opcode_i64_mul = 126,
    cwasm_opcode_i64_div_s = 127,
    cwasm_opcode_i64_div_u = 128,
    cwasm_opcode_i64_rem_s = 129,
    cwasm_opcode_i64_rem_u = 130,
    cwasm_opcode_i64_and = 131,
    cwasm_opcode_i64_or = 132,
    cwasm_opcode_i64_xor = 133,
    cwasm_opcode_i64_shl = 134,
    cwasm_opcode_i64_shr_s = 135,
    cwasm_opcode_i64_shr_u = 136,
    cwasm_opcode_i64_rotl = 137,
    cwasm_opcode_i64_rotr = 138,

    cwasm_opcode_f32_abs = 139,
    cwasm_opcode_f32_neg = 140,
    cwasm_opcode_f32_ceil = 141,
    cwasm_opcode_f32_floor = 142,
    cwasm_opcode_f32_trunc = 143,
    cwasm_opcode_f32_nearest = 144,
    cwasm_opcode_f32_sqrt = 145,
    cwasm_opcode_f32_add = 146,
    cwasm_opcode_f32_sub = 147,
    cwasm_opcode_f32_mul = 148,
    cwasm_opcode_f32_div = 149,
    cwasm_opcode_f32_min = 150,
    cwasm_opcode_f32_max = 151,
    cwasm_opcode_f32_copysign = 152,

    cwasm_opcode_f64_abs = 153,
    cwasm_opcode_f64_neg = 154,
    cwasm_opcode_f64_ceil = 155,
    cwasm_opcode_f64_floor = 156,
    cwasm_opcode_f64_trunc = 157,
    cwasm_opcode_f64_nearest = 158,
    cwasm_opcode_f64_sqrt = 159,
    cwasm_opcode_f64_add = 160,
    cwasm_opcode_f64_sub = 161,
    cwasm_opcode_f64_mul = 162,
    cwasm_opcode_f64_div = 163,
    cwasm_opcode_f64_min = 164,
    cwasm_opcode_f64_max = 165,
    cwasm_opcode_f64_copysign = 166,

    cwasm_opcode_i32_wrap_i64 = 167,
    cwasm_opcode_i32_trunc_f32_s = 168,
    cwasm_opcode_i32_trunc_f32_u = 169,
    cwasm_opcode_i32_trunc_f64_s = 170,
    cwasm_opcode_i32_trunc_f64_u = 171,
    cwasm_opcode_i64_extend_i32_s = 172,
    cwasm_opcode_i64_extend_i32_u = 173,
    cwasm_opcode_i64_trunc_f32_s = 174,
    cwasm_opcode_i64_trunc_f32_u = 175,
    cwasm_opcode_i64_trunc_f64_s = 176,
    cwasm_opcode_i64_trunc_f64_u = 177,
    cwasm_opcode_f32_convert_i32_s = 178,
    cwasm_opcode_f32_convert_i32_u = 179,
    cwasm_opcode_f32_convert_i64_s = 180,
    cwasm_opcode_f32_convert_i64_u = 181,
    cwasm_opcode_f32_demote_f64 = 182,
    cwasm_opcode_f64_convert_i32_s = 183,
    cwasm_opcode_f64_convert_i32_u = 184,
    cwasm_opcode_f64_convert_i64_s = 185,
    cwasm_opcode_f64_convert_i64_u = 186,
    cwasm_opcode_f64_promote_f32 = 187,
    cwasm_opcode_i32_reinterpret_f32 = 188,
    cwasm_opcode_i64_reinterpret_f64 = 189,
    cwasm_opcode_f32_reinterpret_f32 = 190,
    cwasm_opcode_f64_reinterpret_f64 = 191,

    cwasm_opcode_ref_null = 208
};
