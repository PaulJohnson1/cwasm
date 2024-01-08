#pragma once

#include <stdint.h>

struct proto_bug;

struct cwasm_type_external
{
    uint64_t data;
    uint8_t type;
};

// clone because small
extern int cwasm_type_external_write(struct cwasm_type_external,
                                     struct proto_bug *);
extern int cwasm_type_external_read(struct cwasm_type_external *,
                                    struct proto_bug *);
