#include <escarp.h>

#include <assert.h>
#include <stdlib.h>

typedef struct escarp_value_t {
    escarp_parser_t base;
    unsigned char value;
} escarp_value_t;

static escarp_parser_error_t parse(escarp_parser_t *base, escarp_stream_t *in,
                                   escarp_stream_t **out) {
    escarp_value_t *parser = (escarp_value_t *)base;
    escarp_stream_t *out_buf = NULL;
    unsigned char out_value = 0;
    escarp_stream_error_t err = ESCARP_PARSER_SUCCESS;

    assert(parser != NULL);
    assert(in != NULL);
    assert(out != NULL);

    err = escarp_stream_read(in, &out_value);
    switch (err) {
    case ESCARP_STREAM_SUCCESS:
        break;
    case ESCARP_STREAM_ERROR_NULL_POINTER:
        assert(0);
        break;
    case ESCARP_STREAM_ERROR_MEMORY_ALLOCATION_FAILURE:
        return ESCARP_PARSER_ERROR_MEMORY_ALLOCATION_FAILURE;
    case ESCARP_STREAM_ERROR_EOS:
        return ESCARP_PARSER_ERROR_UNEXPECTED_EOS;
    default:
        assert(0);
        break;
    }

    if (out_value != parser->value) {
        return ESCARP_PARSER_ERROR_UNEXPECTED_VALUE;
    }

    out_buf = escarp_uint8_stream_from(&out_value, 1);
    if (out_buf == NULL) {
        return ESCARP_PARSER_ERROR_MEMORY_ALLOCATION_FAILURE;
    }

    *out = out_buf;
    return ESCARP_PARSER_SUCCESS;
}

static void delete_(escarp_parser_t *base) {
    escarp_value_t *parser = (escarp_value_t *)base;

    assert(parser != NULL);

    free(parser);
}

escarp_parser_t *escarp_value(unsigned char value) {
    static escarp_parser_vtable_t vtbl = {parse, delete_};

    escarp_value_t *parser = (escarp_value_t *)malloc(sizeof(escarp_value_t));
    if (parser == NULL) {
        return NULL;
    }

    parser->base.vtbl = &vtbl;
    parser->value = value;

    return (escarp_parser_t *)parser;
}