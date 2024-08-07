#ifndef ESCARP_H
#define ESCARP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <stdio.h>

typedef enum escarp_error_t {
    ESCARP_SUCCESS,

    ESCARP_ERROR_UNEXPECTED_EOF,
    ESCARP_ERROR_UNEXPECTED_VALUE
} escarp_error_t;

typedef struct escarp_stream_t {
    int (*getc)(struct escarp_stream_t *);

    int *history;
    size_t history_count_limit;
    size_t history_count;
    size_t history_index;

    size_t *checkpoints;
    size_t checkpoints_count_limit;
    size_t checkpoints_count;

} escarp_stream_t;

void escarp_stream_init(escarp_stream_t *, int *, size_t, size_t *, size_t);
int escarp_getc(escarp_stream_t *);
int escarp_with_backtracking(escarp_stream_t *,
                             int (*)(escarp_stream_t *, void *), void *);

typedef struct escarp_parser_t {
    escarp_error_t (*parse)(struct escarp_parser_t *, escarp_stream_t *,
                            void *);
} escarp_parser_t;

#define escarp_parse(parser, stream, out)                                      \
    (assert((parser) != NULL), assert((stream) != NULL),                       \
     assert((out) != NULL), (parser)->parse((parser), (stream), (out)))

typedef struct escarp_any_t {
    escarp_parser_t base;
} escarp_any_t;

escarp_parser_t *escarp_any(escarp_any_t *);

typedef struct escarp_value_t {
    escarp_parser_t base;
    unsigned char value;
} escarp_value_t;

escarp_parser_t *escarp_value(escarp_value_t *, unsigned char);

typedef struct escarp_sequence_t {
    escarp_parser_t base;
    escarp_parser_t *first;
    escarp_parser_t *second;
} escarp_sequence_t;

escarp_parser_t *escarp_sequence(escarp_sequence_t *, escarp_parser_t *,
                                 escarp_parser_t *);

typedef struct escarp_repeat_t {
    escarp_parser_t base;
    escarp_parser_t *target;
    size_t min;
    size_t max;
} escarp_repeat_t;

escarp_parser_t *escarp_repeat(escarp_repeat_t *, escarp_parser_t *, size_t,
                               size_t);

typedef struct escarp_choice_t {
    escarp_parser_t base;
    escarp_parser_t *first;
    escarp_parser_t *second;
} escarp_choice_t;

escarp_parser_t *escarp_choice(escarp_choice_t *, escarp_parser_t *,
                               escarp_parser_t *);

typedef struct escarp_ignore_t {
    escarp_parser_t base;
    escarp_parser_t *target;
} escarp_ignore_t;

escarp_parser_t *escarp_ignore(escarp_ignore_t *, escarp_parser_t *);

#ifdef __cplusplus
}
#endif

#endif /* ESCARP_H */