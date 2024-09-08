#include "pb.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef PROTO_BUG_WINDOWS
#include <alloca.h>
#define PROTO_BUG_ALLOCA(size) alloca(size)
#else
#include <malloc.h>
#define PROTO_BUG_ALLOCA(size) _alloca(size)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
#define ENCODING_TYPES                                                         \
    XX(uint8)                                                                  \
    XX(int8)                                                                   \
    XX(uint16)                                                                 \
    XX(uint32)                                                                 \
    XX(uint64)                                                                 \
    XX(varuint)                                                                \
    XX(varint32)                                                               \
    XX(varint64)                                                               \
    XX(float32)                                                                \
    XX(float64)                                                                \
    XX(string)

    enum encoding_type
    {
#define XX(n) n,
        ENCODING_TYPES
#undef XX
    };

    char const *get_encoding_type_string(enum encoding_type encoding_type)
    {
        switch (encoding_type)
        {
#define XX(n)                                                                  \
    case n:                                                                    \
        return #n;
            ENCODING_TYPES
#undef XX
        default:
            assert(0);
            __builtin_unreachable();
        }
    }

    void proto_bug_init(struct proto_bug *self, uint8_t *data)
    {
        self->offset = 0;
        self->start = data;
        self->current = data;
    }

    void proto_bug_reset(struct proto_bug *self)
    {
        self->current = self->start;
    }

    uint64_t proto_bug_get_size(struct proto_bug *self)
    {
        return self->current - self->start;
    }

    uint64_t proto_bug_get_total_size(struct proto_bug *self)
    {
        return self->offset + proto_bug_get_size(self);
    }

    void proto_bug_write_uint8_internal(struct proto_bug *self, uint8_t data)
    {
        *self->current++ = data;
    }
    void proto_bug_write_uint16_internal(struct proto_bug *self, uint16_t data)
    {
        proto_bug_write_uint8_internal(self, data & 0xff);
        proto_bug_write_uint8_internal(self, (data & 0xff00) >> 8);
    }
    void proto_bug_write_uint32_internal(struct proto_bug *self, uint32_t data)
    {
        proto_bug_write_uint8_internal(self, data & 0xff);
        proto_bug_write_uint8_internal(self, (data & 0x0000ff00) >> 8);
        proto_bug_write_uint8_internal(self, (data & 0x00ff0000) >> 16);
        proto_bug_write_uint8_internal(self, (data & 0xff000000) >> 24);
    }
    void proto_bug_write_uint64_internal(struct proto_bug *self, uint64_t data)
    {
        proto_bug_write_uint8_internal(self, data & 0xffull);
        proto_bug_write_uint8_internal(self, (data & 0xff00ull) >> 8ull);
        proto_bug_write_uint8_internal(self, (data & 0xff0000ull) >> 16ull);
        proto_bug_write_uint8_internal(self, (data & 0xff000000ull) >> 24ull);
        proto_bug_write_uint8_internal(self, (data & 0xff00000000ull) >> 32ull);
        proto_bug_write_uint8_internal(self,
                                       (data & 0xff0000000000ull) >> 40ull);
        proto_bug_write_uint8_internal(self,
                                       (data & 0xff000000000000ull) >> 48ull);
        proto_bug_write_uint8_internal(self,
                                       (data & 0xff00000000000000ull) >> 56ull);
    }
    void proto_bug_write_float32_internal(struct proto_bug *self, float data)
    {
        memcpy(self->current, &data,
               sizeof data); // the compiler is a genius and optimizes self
        self->current += sizeof data;
    }
    void proto_bug_write_float64_internal(struct proto_bug *self, double data)
    {
        memcpy(self->current, &data, sizeof data);
        self->current += sizeof data;
    }
    void proto_bug_write_varuint_internal(struct proto_bug *self, uint64_t data)
    {
        while (data > 127ull)
        {
            proto_bug_write_uint8_internal(self, data | 128ull);
            data >>= 7ull;
        }
        proto_bug_write_uint8_internal(self, data);
    }
    void proto_bug_write_varint32_internal(struct proto_bug *self, int32_t data)
    {
        while (1)
        {
            int32_t byte = data & 0x7f;
            data >>= 7;

            if ((data == -1 && (byte & 0x40)) || (data == 0 && !(byte & 0x40)))
            {
                *self->current++ = byte;
                break;
            }
            else
                *self->current++ = byte | 0x80;
        }
    }
    void proto_bug_write_varint64_internal(struct proto_bug *self, int64_t data)
    {
        while (1)
        {
            int32_t byte = data & 0x7fll;
            data >>= 7ll;

            if ((data == -1ll && (byte & 0x40ll)) ||
                (data == 0ll && !(byte & 0x40ll)))
            {
                *self->current++ = byte;
                break;
            }
            else
                *self->current++ = byte | 0x80ll;
        }
    }
    void proto_bug_write_string_internal(struct proto_bug *self,
                                         char const *string, uint64_t size)
    {
        for (uint64_t i = 0; i < size; i++)
            proto_bug_write_uint8_internal(self, string[i]);
    }

    uint8_t proto_bug_read_uint8_internal(struct proto_bug *self)
    {
        return *self->current++;
    }
    uint16_t proto_bug_read_uint16_internal(struct proto_bug *self)
    {
        uint16_t data = 0;
        data |= (uint16_t)proto_bug_read_uint8_internal(self);
        data |= (uint16_t)proto_bug_read_uint8_internal(self) << 8;

        return data;
    }
    uint32_t proto_bug_read_uint32_internal(struct proto_bug *self)
    {
        uint32_t data = 0;
        data |= (uint32_t)proto_bug_read_uint8_internal(self);
        data |= (uint32_t)proto_bug_read_uint8_internal(self) << 8;
        data |= (uint32_t)proto_bug_read_uint8_internal(self) << 16;
        data |= (uint32_t)proto_bug_read_uint8_internal(self) << 24;

        return data;
    }
    uint64_t proto_bug_read_uint64_internal(struct proto_bug *self)
    {
        uint64_t data = 0;
        data |= (uint64_t)proto_bug_read_uint8_internal(self);
        data |= (uint64_t)proto_bug_read_uint8_internal(self) << 8ull;
        data |= (uint64_t)proto_bug_read_uint8_internal(self) << 16ull;
        data |= (uint64_t)proto_bug_read_uint8_internal(self) << 24ull;
        data |= (uint64_t)proto_bug_read_uint8_internal(self) << 32ull;
        data |= (uint64_t)proto_bug_read_uint8_internal(self) << 40ull;
        data |= (uint64_t)proto_bug_read_uint8_internal(self) << 48ull;
        data |= (uint64_t)proto_bug_read_uint8_internal(self) << 56ull;

        return data;
    }
    float proto_bug_read_float32_internal(struct proto_bug *self)
    {
        float data;
        memcpy(&data, self->current, sizeof data);
        self->current += sizeof data;
        return data;
    }
    double proto_bug_read_float64_internal(struct proto_bug *self)
    {
        double data;
        memcpy(&data, self->current, sizeof data);
        self->current += sizeof data;
        return data;
    }
    uint64_t proto_bug_read_varuint_internal(struct proto_bug *self)
    {
        uint8_t byte;
        uint64_t data = 0ull;
        uint64_t shift = 0ull;

        do
        {
            byte = proto_bug_read_uint8_internal(self);
            data |= (byte & 127ull) << shift;
            shift += 7ull;
        } while (byte & 128ull);

        return data;
    }
    int32_t proto_bug_read_varint32_internal(struct proto_bug *self)
    {
        int32_t x = 0;
        int32_t shift = 0;
        while (1)
        {
            int32_t byte = proto_bug_read_uint8_internal(self);
            x |= (byte & 127) << shift;
            shift += 7;

            if ((byte & 128) == 0)
                break;
        }

        if (shift >= 32)
            return x;
        int32_t ashift = 32 - shift;
        return (x << ashift) >> ashift;
    }
    int64_t proto_bug_read_varint64_internal(struct proto_bug *self)
    {
        int64_t x = 0;
        int64_t shift = 0;
        while (1)
        {
            int64_t byte = proto_bug_read_uint8_internal(self);
            x |= (byte & 127ll) << shift;
            shift += 7ll;

            if ((byte & 128ll) == 0ll)
            {
                if (shift < 32ll && (byte & 40ll) != 0ll)
                    return x | (~0ll << shift);
                return x;
            }
        }
    }
    void proto_bug_read_string_internal(struct proto_bug *self, char *string,
                                        uint64_t size)
    {
        for (uint64_t i = 0; i < size; i++)
            string[i] = proto_bug_read_uint8_internal(self);
    }

    static char assertion_fail_message[1024];

    void proto_bug_assert_valid_debug_header(
        struct proto_bug *self, enum encoding_type expected_encoding_type,
        char const *name, char const *file, uint32_t line)
    {
        uint64_t magic = proto_bug_read_uint64_internal(self);
        if (magic != 0x1234567890abcdefull)
        {
            assertion_fail_message[sprintf(
                assertion_fail_message,
                "proto_bug exception: read invalid data (maybe OOB)\n"
                "invalid read at: %s:%u\n"
                "expected: %" PRId64 "X; encountered: %" PRId64 "X\n",
                file, line, 0x1234567890abcdeflu, magic)] = 0;

            fputs(assertion_fail_message, stderr);
            abort();
        }
        // explicit casting because of c++
        enum encoding_type received_encoding_type =
            (enum encoding_type)proto_bug_read_uint8_internal(self);
        uint64_t name_size = proto_bug_read_varuint_internal(self);
        uint64_t file_size = proto_bug_read_varuint_internal(self);
        char *received_name = (char *)PROTO_BUG_ALLOCA(name_size + 1);
        char *received_file = (char *)PROTO_BUG_ALLOCA(file_size + 1);
        memset(received_name, 0, name_size + 1);
        memset(received_file, 0, file_size + 1);
        proto_bug_read_string_internal(self, received_name, name_size);
        proto_bug_read_string_internal(self, received_file, file_size);
        uint32_t received_line = proto_bug_read_varuint_internal(self);
        if ((received_encoding_type != expected_encoding_type) ||
            ((name_size != strlen(name) || (strcmp(received_name, name)))))
        {
            assertion_fail_message[sprintf(
                assertion_fail_message,
                "proto_bug exception: read/write debug information does not "
                "match.\n"
                "expected:    encoding_type: %s\tname: %s\t   read_at: %s:%u\n"
                "encountered: encoding_type: %s\tname: %s\twritten_at: %s:%u\n",
                get_encoding_type_string(expected_encoding_type), name, file,
                line, get_encoding_type_string(received_encoding_type),
                received_name, received_file, received_line)] = 0;

            fputs(assertion_fail_message, stderr);
            abort();
        }
    }

    void proto_bug_write_debug_header(struct proto_bug *self,
                                      enum encoding_type encoding_type,
                                      char const *name, char const *file,
                                      uint32_t line)
    {
        proto_bug_write_uint64_internal(self, 0x1234567890abcdefull);
        proto_bug_write_uint8_internal(self, encoding_type);
        uint64_t name_size = strlen(name);
        uint64_t file_size = strlen(file);
        proto_bug_write_varuint_internal(self, name_size);
        proto_bug_write_varuint_internal(self, file_size);
        proto_bug_write_string_internal(self, name, name_size);
        proto_bug_write_string_internal(self, file, file_size);
        proto_bug_write_varuint_internal(self, line);
    }

    void proto_bug_write_uint8_debug(struct proto_bug *self, uint8_t data,
                                     char const *name, char const *file,
                                     uint32_t line)
    {
        proto_bug_write_debug_header(self, uint8, name, file, line);
        proto_bug_write_uint8_internal(self, data);
    }
    void proto_bug_write_uint16_debug(struct proto_bug *self, uint16_t data,
                                      char const *name, char const *file,
                                      uint32_t line)
    {
        proto_bug_write_debug_header(self, uint16, name, file, line);
        proto_bug_write_uint16_internal(self, data);
    }
    void proto_bug_write_uint32_debug(struct proto_bug *self, uint32_t data,
                                      char const *name, char const *file,
                                      uint32_t line)
    {
        proto_bug_write_debug_header(self, uint32, name, file, line);
        proto_bug_write_uint32_internal(self, data);
    }
    void proto_bug_write_uint64_debug(struct proto_bug *self, uint64_t data,
                                      char const *name, char const *file,
                                      uint32_t line)
    {
        proto_bug_write_debug_header(self, uint64, name, file, line);
        proto_bug_write_uint64_internal(self, data);
    }
    void proto_bug_write_float32_debug(struct proto_bug *self, float data,
                                       char const *name, char const *file,
                                       uint32_t line)
    {
        proto_bug_write_debug_header(self, float32, name, file, line);
        proto_bug_write_float32_internal(self, data);
    }
    void proto_bug_write_float64_debug(struct proto_bug *self, double data,
                                       char const *name, char const *file,
                                       uint32_t line)
    {
        proto_bug_write_debug_header(self, float64, name, file, line);
        proto_bug_write_float64_internal(self, data);
    }
    void proto_bug_write_varuint_debug(struct proto_bug *self, uint64_t data,
                                       char const *name, char const *file,
                                       uint32_t line)
    {
        proto_bug_write_debug_header(self, varuint, name, file, line);
        proto_bug_write_varuint_internal(self, data);
    }
    void proto_bug_write_varint32_debug(struct proto_bug *self, int32_t data,
                                        char const *name, char const *file,
                                        uint32_t line)
    {
        proto_bug_write_debug_header(self, varint32, name, file, line);
        proto_bug_write_varint32_internal(self, data);
    }
    void proto_bug_write_varint64_debug(struct proto_bug *self, int64_t data,
                                        char const *name, char const *file,
                                        uint32_t line)
    {
        proto_bug_write_debug_header(self, varint64, name, file, line);
        proto_bug_write_varint64_internal(self, data);
    }
    void proto_bug_write_string_debug(struct proto_bug *self,
                                      char const *string_pointer, uint64_t size,
                                      char const *name, char const *file,
                                      uint32_t line)
    {
        proto_bug_write_debug_header(self, string, name, file, line);
        proto_bug_write_string_internal(self, string_pointer, size);
    }

    uint8_t proto_bug_read_uint8_debug(struct proto_bug *self, char const *name,
                                       char const *file, uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, uint8, name, file, line);
        return proto_bug_read_uint8_internal(self);
    }
    uint16_t proto_bug_read_uint16_debug(struct proto_bug *self,
                                         char const *name, char const *file,
                                         uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, uint16, name, file, line);
        return proto_bug_read_uint16_internal(self);
    }
    uint32_t proto_bug_read_uint32_debug(struct proto_bug *self,
                                         char const *name, char const *file,
                                         uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, uint32, name, file, line);
        return proto_bug_read_uint32_internal(self);
    }
    uint64_t proto_bug_read_uint64_debug(struct proto_bug *self,
                                         char const *name, char const *file,
                                         uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, uint64, name, file, line);
        return proto_bug_read_uint64_internal(self);
    }
    float proto_bug_read_float32_debug(struct proto_bug *self, char const *name,
                                       char const *file, uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, float32, name, file, line);
        return proto_bug_read_float32_internal(self);
    }
    double proto_bug_read_float64_debug(struct proto_bug *self,
                                        char const *name, char const *file,
                                        uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, float64, name, file, line);
        return proto_bug_read_float64_internal(self);
    }
    uint64_t proto_bug_read_varuint_debug(struct proto_bug *self,
                                          char const *name, char const *file,
                                          uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, varuint, name, file, line);
        return proto_bug_read_varuint_internal(self);
    }
    int32_t proto_bug_read_varint32_debug(struct proto_bug *self,
                                          char const *name, char const *file,
                                          uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, varint32, name, file, line);
        return proto_bug_read_varint32_internal(self);
    }
    int64_t proto_bug_read_varint64_debug(struct proto_bug *self,
                                          char const *name, char const *file,
                                          uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, varint64, name, file, line);
        return proto_bug_read_varint64_internal(self);
    }
    void proto_bug_read_string_debug(struct proto_bug *self,
                                     char *string_pointer, uint64_t size,
                                     char const *name, char const *file,
                                     uint32_t line)
    {
        proto_bug_assert_valid_debug_header(self, string, name, file, line);
        proto_bug_read_string_internal(self, string_pointer, size);
    }

#ifdef __cplusplus
}
#endif
