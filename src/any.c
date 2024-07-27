#include <escarp.h>

#include "internal.h"

static escarp_error_t any_parse(escarp_parser_t *base, FILE *fp, void *out) {
    int c = EOF;

    escarp_unused(base);

    if ((c = fgetc(fp)) == EOF) {
        return ESCARP_ERROR_UNEXPECTED_EOF;
    }

    ((int *)out)[0] = c;
    ((int *)out)[1] = EOF;

    return ESCARP_SUCCESS;
}

escarp_parser_t *escarp_any(escarp_any_t *self) {
    return self == NULL ? NULL : (self->base.parse = any_parse, &(self->base));
}
