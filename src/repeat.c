#include <escarp.h>

#include "internal.h"

typedef struct repeat_context_t {
    escarp_parser_t *target;
    int *out;
} repeat_context_t;

static int callback(escarp_stream_t *stream, void *other_data) {
    repeat_context_t *ctx = (repeat_context_t *)other_data;
    return escarp_parse(ctx->target, stream, ctx->out) == ESCARP_SUCCESS ? 1
                                                                         : 0;
}

static escarp_error_t repeat_parse(escarp_parser_t *base,
                                   escarp_stream_t *stream, void *out) {
    escarp_repeat_t *self = (escarp_repeat_t *)base;
    escarp_error_t err = ESCARP_SUCCESS;
    size_t idx = 0;
    long pos = 0;
    size_t i = 0;

    for (i = 0; i < self->min; i++) {
        if ((err = escarp_parse(self->target, stream, &((int *)out)[idx])) !=
            ESCARP_SUCCESS) {
            return err;
        }
        for (; ((int *)out)[idx] != EOF; idx++)
            ;
    }

    for (i = 0; i < self->max - self->min; i++) {
        repeat_context_t ctx = {self->target, &((int *)out)[idx]};
        if (!escarp_with_backtracking(stream, callback, &ctx)) {
            ((int *)out)[idx] = EOF;
            return ESCARP_SUCCESS;
        }
        for (; ((int *)out)[idx] != EOF; idx++)
            ;
    }

    ((int *)out)[idx] = EOF;
    return ESCARP_SUCCESS;
}

escarp_parser_t *escarp_repeat(escarp_repeat_t *self, escarp_parser_t *target,
                               size_t min, size_t max) {
    return (self == NULL || target == NULL || max < min)
               ? NULL
               : (self->base.parse = repeat_parse, self->target = target,
                  self->min = min, self->max = max, &(self->base));
}
