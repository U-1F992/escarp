#include <escarp.h>

#include <assert.h>
#include <stdlib.h>

typedef struct escarp_value_t {
    escarp_parser_t base;
    unsigned char value;
} escarp_value_t;

static escarp_parser_error_t parse(escarp_parser_t *base, escarp_stream_t *in,
                                   void *out) {
    escarp_value_t *parser = (escarp_value_t *)base;
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

    if (value != parser->value) {
        return ESCARP_PARSER_ERROR_UNEXPECTED_VALUE;
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