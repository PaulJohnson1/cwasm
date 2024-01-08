#pragma once

#include <stdint.h>

struct proto_bug;

struct type_external
{
    uint64_t data;
    uint8_t type;
};

// clone because small
extern int type_external_write(struct type_external, struct proto_bug *);
extern int type_external_read(struct type_external *, struct proto_bug *);
