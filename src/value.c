#include <escarp.h>

#include "internal.h"

static escarp_error_t value_parse(escarp_parser_t *base, FILE *fp, void *out) {
    escarp_value_t *self = (escarp_value_t *)base;
    int c = EOF;

    if ((c = fgetc(fp)) == EOF) {
        return ESCARP_ERROR_UNEXPECTED_EOF;
    } else if (c != self->value) {
        return ESCARP_ERROR_UNEXPECTED_VALUE;
    }

    ((int *)out)[0] = c;
    ((int *)out)[1] = EOF;

    return ESCARP_SUCCESS;
}

escarp_parser_t *escarp_value(escarp_value_t *self, unsigned char value) {
    return self == NULL ? NULL
                        : (self->base.parse = value_parse, self->value = value,
                           &(self->base));
}
