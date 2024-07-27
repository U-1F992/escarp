#include <escarp.h>

#include "internal.h"

static escarp_error_t choice_parse(escarp_parser_t *base, FILE *fp, void *out) {
    escarp_choice_t *self = (escarp_choice_t *)base;
    escarp_error_t err[2];
    long pos = 0;

    pos = ftell(fp);
    if ((err[0] = escarp_parse(self->first, fp, out)) == ESCARP_SUCCESS) {
        return ESCARP_SUCCESS;
    }
    fseek(fp, pos, SEEK_SET);
    if ((err[1] = escarp_parse(self->second, fp, out)) == ESCARP_SUCCESS) {
        return ESCARP_SUCCESS;
    }

    return (err[0] == ESCARP_ERROR_UNEXPECTED_EOF && err[1] == err[0])
               ? err[0]
               : ESCARP_ERROR_UNEXPECTED_VALUE;
}

escarp_parser_t *escarp_choice(escarp_choice_t *self, escarp_parser_t *first,
                               escarp_parser_t *second) {
    return (self == NULL || first == NULL || second == NULL)
               ? NULL
               : (self->base.parse = choice_parse, self->first = first,
                  self->second = second, &(self->base));
}
