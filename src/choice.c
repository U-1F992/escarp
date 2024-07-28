#include <escarp.h>

#include "internal.h"

typedef struct choice_context_t {
    escarp_parser_t *target;
    int *out;
    escarp_error_t out_err;
} choice_context_t;

static int callback(escarp_stream_t *stream, void *other_data) {
    choice_context_t *ctx = (choice_context_t *)other_data;
    return (ctx->out_err = escarp_parse(ctx->target, stream, ctx->out)) ==
                   ESCARP_SUCCESS
               ? 1
               : 0;
}

static escarp_error_t choice_parse(escarp_parser_t *base,
                                   escarp_stream_t *stream, void *out) {
    escarp_choice_t *self = (escarp_choice_t *)base;
    escarp_error_t err;

    choice_context_t ctx = {self->first, out, ESCARP_SUCCESS};
    if (escarp_use_backtracking(stream, callback, &ctx) ||
        (err = escarp_parse(self->second, stream, out)) == ESCARP_SUCCESS) {
        return ESCARP_SUCCESS;
    }

    return (ctx.out_err == ESCARP_ERROR_UNEXPECTED_EOF && err == ctx.out_err)
               ? ESCARP_ERROR_UNEXPECTED_EOF
               : ESCARP_ERROR_UNEXPECTED_VALUE;
}

escarp_parser_t *escarp_choice(escarp_choice_t *self, escarp_parser_t *first,
                               escarp_parser_t *second) {
    return (self == NULL || first == NULL || second == NULL)
               ? NULL
               : (self->base.parse = choice_parse, self->first = first,
                  self->second = second, &(self->base));
}
