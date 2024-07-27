#include <escarp.h>

#include "internal.h"

static escarp_error_t sequence_parse(escarp_parser_t *base, FILE *fp,
                                     void *out) {
    escarp_sequence_t *self = (escarp_sequence_t *)base;
    escarp_error_t err = ESCARP_SUCCESS;
    size_t idx = 0;

    if ((err = escarp_parse(self->first, fp, &((int *)out)[idx])) !=
        ESCARP_SUCCESS) {
        return err;
    }
    for (; ((int *)out)[idx] != EOF; idx++)
        ;
    if ((err = escarp_parse(self->second, fp, &((int *)out)[idx])) !=
        ESCARP_SUCCESS) {
        return err;
    }

    return ESCARP_SUCCESS;
}

escarp_parser_t *escarp_sequence(escarp_sequence_t *self,
                                 escarp_parser_t *first,
                                 escarp_parser_t *second) {
    return (self == NULL || first == NULL || second == NULL)
               ? NULL
               : (self->base.parse = sequence_parse, self->first = first,
                  self->second = second, &(self->base));
}
