#ifndef ESCARP_H
#define ESCARP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * Definition only for compatibility.
 * We strongly recommend using the native `uint8_t` instead of `escarp_uint8_t`.
 */
typedef unsigned char escarp_uint8_t;

typedef enum escarp_error_t {
    ESCARP_SUCCESS,
    ESCARP_ERROR_NULL_POINTER,
    ESCARP_ERROR_MEMORY_ALLOCATION_FAILURE,

    ESCARP_PARSER_ERROR_UNEXPECTED_EOF,
    ESCARP_PARSER_ERROR_UNEXPECTED_VALUE
} escarp_error_t;

typedef struct escarp_parser_t escarp_parser_t;

typedef struct escarp_parser_vtable_t {
    escarp_error_t (*parse)(struct escarp_parser_t *, escarp_uint8_t *,
                            size_t *, size_t, void **, size_t *);
    void (*delete_)(struct escarp_parser_t *);
} escarp_parser_vtable_t;

struct escarp_parser_t {
    escarp_parser_vtable_t *vtbl;
};

escarp_error_t escarp_parse(escarp_parser_t *, escarp_uint8_t *, size_t *,
                            size_t, void **, size_t *);
void escarp_parser_delete(escarp_parser_t *);

escarp_parser_t *escarp_any(void);
escarp_parser_t *escarp_value(escarp_uint8_t);
escarp_parser_t *escarp_sequence(size_t, ...);

#ifdef __cplusplus
}
#endif

#endif /* ESCARP_H */