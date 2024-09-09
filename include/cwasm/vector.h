#pragma once

#define cwasm_vector_declare(type, name)                                       \
    type *name;                                                                \
    type *name##_end;                                                          \
    type *name##_cap;

#define cwasm_vector_grow(type, name)                                          \
    if (name##_end >= name##_cap)                                              \
    {                                                                          \
        uint64_t capacity = name##_cap - name;                                 \
        type *new_data = realloc(name, (capacity * 2 + 1) * sizeof *name);     \
        type *new_data_cap = new_data + capacity * 2 + 1;                      \
        name = new_data;                                                       \
        name##_end = new_data + capacity;                                      \
        name##_cap = new_data_cap;                                             \
    }
