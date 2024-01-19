#pragma once

#include <stdint.h>

#include <consts.h>

struct proto_bug;

enum cwasm_type_description_type
{
    cwasm_type_description_type_function,
    cwasm_type_description_type_table,
    cwasm_type_description_type_memory,
    cwasm_type_description_type_global,
};

struct cwasm_type_limit
{
    uint64_t min;
    uint64_t max;
};

struct cwasm_type_table
{
    struct cwasm_type_limit limit;
    int8_t reference_type;
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

struct cwasm_type_description
{
    enum cwasm_type_description_type type;
    union
    {
        uint64_t table_index;
        struct cwasm_type_table table;
        struct cwasm_type_memory memory;
        struct cwasm_type_global global;
    };
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

extern void cwasm_type_description_write(struct cwasm_type_description *,
                                         struct proto_bug *);
extern void cwasm_type_description_read(struct cwasm_type_description *,
                                        struct proto_bug *);
