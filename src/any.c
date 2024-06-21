#include <escarp.h>

#include <assert.h>
#include <stdlib.h>

static escarp_error_t parse(escarp_parser_t *parser, escarp_uint8_t *in,
                            size_t *in_idx, size_t in_len, void **out,
                            size_t *out_len) {
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
    *out_buf = in[(*in_idx)++];
    *out = (void *)out_buf;
    *out_len = 1;
    return ESCARP_SUCCESS;
}

static void delete_(escarp_parser_t *parser) {
    assert(parser != NULL);
    /* Nothing to do, since the parser is allocated statically. */
}

escarp_parser_t *escarp_any(void) {
    static escarp_parser_vtable_t vtbl = {parse, delete_};
    static escarp_parser_t parser = {&vtbl};

    return &parser;
}
