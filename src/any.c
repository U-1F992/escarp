#include <escarp.h>

#include <assert.h>
#include <stdlib.h>

static escarp_parser_error_t parse(escarp_parser_t *parser, escarp_stream_t *in,
                                   void *out) {
    unsigned char value = 0;
    escarp_array_t *new_out = NULL;

    assert(parser != NULL);
    assert(in != NULL);
    assert(out != NULL);

    switch (escarp_stream_read(in, &value)) {
    case ESCARP_STREAM_SUCCESS:
        break;
    case ESCARP_STREAM_ERROR_MEMORY_ALLOCATION_FAILURE:
        return ESCARP_PARSER_ERROR_MEMORY_ALLOCATION_FAILURE;
    case ESCARP_STREAM_ERROR_EOS:
        return ESCARP_PARSER_ERROR_UNEXPECTED_EOS;
    default:
        assert(0);
        break;
    }

    switch (escarp_array_new(&new_out, sizeof(unsigned char), &value, 1)) {
    case ESCARP_ARRAY_SUCCESS:
        break;
    case ESCARP_ARRAY_ERROR_MEMORY_ALLOCATION_FAILURE:
        return ESCARP_PARSER_ERROR_MEMORY_ALLOCATION_FAILURE;
    default:
        assert(0);
        break;
    }
    assert(new_out != NULL);

    *(escarp_array_t **)out = new_out;
    return ESCARP_PARSER_SUCCESS;
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
