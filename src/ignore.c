#include <escarp.h>

#include "internal.h"

static escarp_error_t ignore_parse(escarp_parser_t *base, FILE *fp, void *out) {
    escarp_ignore_t *self = (escarp_ignore_t *)base;
    escarp_error_t err = ESCARP_SUCCESS;

    if ((err = escarp_parse(self->target, fp, out)) != ESCARP_SUCCESS) {
        return err;
    }

    ((int *)out)[0] = EOF;
    return ESCARP_SUCCESS;
}

escarp_parser_t *escarp_ignore(escarp_ignore_t *self, escarp_parser_t *target) {
    return (self == NULL || target == NULL)
               ? NULL
               : (self->base.parse = ignore_parse, self->target = target,
                  &(self->base));
}
