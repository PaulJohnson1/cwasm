#pragma once

#include <stdint.h>

#include <consts.h>

struct proto_bug;

struct cwasm_type_limit
{
    uint64_t min;
    uint64_t max;
};

struct cwasm_type_table
{
    struct cwasm_type_limit limit;
    uint8_t reference_type;
};

struct cwasm_type_memory
{
    struct cwasm_type_limit limit;
};

struct cwasm_type_global
{
    uint64_t flags;
    int8_t value_type;
};

extern void cwasm_type_limit_write(struct cwasm_type_limit *,
                                   struct proto_bug *);
extern void cwasm_type_limit_read(struct cwasm_type_limit *,
                                  struct proto_bug *);

extern void cwasm_type_table_write(struct cwasm_type_table *,
                                   struct proto_bug *);
extern void cwasm_type_table_read(struct cwasm_type_table *,
                                  struct proto_bug *);

extern void cwasm_type_memory_write(struct cwasm_type_memory *,
                                    struct proto_bug *);
extern void cwasm_type_memory_read(struct cwasm_type_memory *,
                                   struct proto_bug *);

extern void cwasm_type_global_write(struct cwasm_type_global *,
                                    struct proto_bug *);
extern void cwasm_type_global_read(struct cwasm_type_global *,
                                   struct proto_bug *);
