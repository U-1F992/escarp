#include <escarp.h>

#include "internal.h"

static escarp_error_t repeat_parse(escarp_parser_t *base, FILE *fp, void *out) {
    escarp_repeat_t *self = (escarp_repeat_t *)base;
    escarp_error_t err = ESCARP_SUCCESS;
    size_t idx = 0;
    long pos = 0;
    size_t i = 0;

    for (i = 0; i < self->min; i++) {
        if ((err = escarp_parse(self->target, fp, &((int *)out)[idx])) !=
            ESCARP_SUCCESS) {
            return err;
        }
        for (; ((int *)out)[idx] != EOF; idx++)
            ;
    }

    for (i = 0; i < self->max - self->min; i++) {
        pos = ftell(fp);
        if ((err = escarp_parse(self->target, fp, &((int *)out)[idx])) !=
            ESCARP_SUCCESS) {
            fseek(fp, pos, SEEK_SET);
            ((int *)out)[idx] = EOF;
            return ESCARP_SUCCESS;
        }
        for (; ((int *)out)[idx] != EOF; idx++)
            ;
    }

    return ESCARP_SUCCESS;
}

escarp_parser_t *escarp_repeat(escarp_repeat_t *self, escarp_parser_t *target,
                               size_t min, size_t max) {
    return (self == NULL || target == NULL || max < min)
               ? NULL
               : (self->base.parse = repeat_parse, self->target = target,
                  self->min = min, self->max = max, &(self->base));
}
