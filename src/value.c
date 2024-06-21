#include <escarp.h>

#include <assert.h>
#include <stdlib.h>

typedef struct escarp_value_t {
    escarp_parser_t base;
    escarp_uint8_t value;
} escarp_value_t;

static escarp_error_t parse(escarp_parser_t *base, escarp_uint8_t *in,
                            size_t *in_idx, size_t in_len, void **out,
                            size_t *out_len) {
    escarp_value_t *parser = (escarp_value_t *)base;
    escarp_uint8_t value = 0;
    escarp_uint8_t *out_buf = NULL;

    assert(parser != NULL);
    assert(in != NULL);
    assert(in_idx != NULL);
    assert(out != NULL);
    assert(out_len != NULL);

    if (*in_idx >= in_len) {
        return ESCARP_PARSER_ERROR_UNEXPECTED_EOF;
    }

    out_buf = (escarp_uint8_t *)malloc(sizeof(escarp_uint8_t));
    if (out_buf == NULL) {
        return ESCARP_ERROR_MEMORY_ALLOCATION_FAILURE;
    }

    value = in[(*in_idx)++];
    if (value != parser->value) {
        free(out_buf);
        return ESCARP_PARSER_ERROR_UNEXPECTED_VALUE;
    }

    *out_buf = value;
    *out = (void *)out_buf;
    *out_len = 1;
    return ESCARP_SUCCESS;
}

static void delete_(escarp_parser_t *base) {
    escarp_value_t *parser = (escarp_value_t *)base;

    assert(parser != NULL);

    free(parser);
}

escarp_parser_t *escarp_value(escarp_uint8_t value) {
    static escarp_parser_vtable_t vtbl = {parse, delete_};
    static escarp_parser_t base = {&vtbl};

    escarp_value_t *parser = (escarp_value_t *)malloc(sizeof(escarp_value_t));
    if (parser == NULL) {
        return NULL;
    }

    parser->base = base;
    parser->value = value;

    return (escarp_parser_t *)parser;
}