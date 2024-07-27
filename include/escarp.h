#ifndef ESCARP_H
#define ESCARP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef enum escarp_array_error_t {
    ESCARP_ARRAY_SUCCESS,
    ESCARP_ARRAY_ERROR_NULL_POINTER,
    ESCARP_ARRAY_ERROR_MEMORY_ALLOCATION_FAILURE,

    ESCARP_ARRAY_ERROR_INVALID_TYPE,
    ESCARP_ARRAY_ERROR_OUT_OF_BOUNDS
} escarp_array_error_t;

typedef struct escarp_array_t escarp_array_t;

escarp_array_error_t escarp_array_new(escarp_array_t **, size_t, void *,
                                      size_t);
void escarp_array_delete(escarp_array_t *);
escarp_array_error_t escarp_array_get_ptr(escarp_array_t *, void **);
escarp_array_error_t escarp_array_get_length(escarp_array_t *, size_t *);
escarp_array_error_t escarp_array_at(escarp_array_t *, size_t, void *);
escarp_array_error_t escarp_array_push(escarp_array_t *, void *);
escarp_array_error_t escarp_array_pop(escarp_array_t *, void *);
escarp_array_error_t escarp_array_concat(escarp_array_t *, escarp_array_t *);

typedef enum escarp_stream_error_t {
    ESCARP_STREAM_SUCCESS,
    ESCARP_STREAM_ERROR_NULL_POINTER,
    ESCARP_STREAM_ERROR_MEMORY_ALLOCATION_FAILURE,

    ESCARP_STREAM_ERROR_EOS
} escarp_stream_error_t;

typedef struct escarp_stream_t escarp_stream_t;

typedef struct escarp_stream_vtable_t {
    escarp_stream_error_t (*read)(struct escarp_stream_t *, void *);
    void (*delete_)(struct escarp_stream_t *);
} escarp_stream_vtable_t;

struct escarp_stream_t {
    escarp_stream_vtable_t *vtbl;

    size_t element_size;

    void *history;
    size_t history_len;
    size_t history_idx;

    size_t *checkpoints;
    size_t checkpoints_len;
};

typedef enum escarp_stream_transaction_status_t {
    ESCARP_STREAM_TRANSACTION_ROLLBACK,
    ESCARP_STREAM_TRANSACTION_COMMIT
} escarp_stream_transaction_status_t;

typedef escarp_stream_transaction_status_t (*escarp_stream_transaction_t)(
    escarp_stream_t *, void *);

escarp_stream_error_t escarp_stream_init(escarp_stream_t *, size_t);
escarp_stream_error_t escarp_stream_read(escarp_stream_t *, void *);
escarp_stream_error_t
escarp_stream_begin_transaction(escarp_stream_t *, escarp_stream_transaction_t,
                                void *);
void escarp_stream_delete(escarp_stream_t *);

escarp_stream_t *escarp_uint8_stream_from(unsigned char *, size_t);

typedef enum escarp_parser_error_t {
    ESCARP_PARSER_SUCCESS,
    ESCARP_PARSER_ERROR_NULL_POINTER,
    ESCARP_PARSER_ERROR_MEMORY_ALLOCATION_FAILURE,

    ESCARP_PARSER_ERROR_UNEXPECTED_EOS,
    ESCARP_PARSER_ERROR_UNEXPECTED_VALUE
} escarp_parser_error_t;

typedef struct escarp_parser_t escarp_parser_t;

typedef struct escarp_parser_vtable_t {
    escarp_parser_error_t (*parse)(struct escarp_parser_t *, escarp_stream_t *,
                                   void *);
    void (*delete_)(struct escarp_parser_t *);
} escarp_parser_vtable_t;

struct escarp_parser_t {
    escarp_parser_vtable_t *vtbl;
};

escarp_parser_error_t escarp_parse(escarp_parser_t *, escarp_stream_t *,
                                   void *);
void escarp_parser_delete(escarp_parser_t *);

escarp_parser_t *escarp_any(void);
escarp_parser_t *escarp_value(unsigned char);
escarp_parser_t *escarp_sequence(size_t, ...);

#ifdef __cplusplus
}
#endif

#endif /* ESCARP_H */