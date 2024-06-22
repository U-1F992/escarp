#include <escarp.h>

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef struct escarp_sequence_t {
    escarp_parser_t base;
    escarp_parser_t **parsers;
    size_t parsers_len;
} escarp_sequence_t;

static escarp_parser_error_t parse(escarp_parser_t *base, escarp_stream_t *in,
                                   escarp_stream_t **out) {
    escarp_sequence_t *parser = (escarp_sequence_t *)base;
    escarp_stream_t *out_tmp = NULL;
    unsigned char *out_arr = NULL;
    size_t out_arr_len = 0;
    size_t i = 0;

    assert(parser != NULL);
    assert(in != NULL);
    assert(out != NULL);

    for (i = 0; i < parser->parsers_len; i++) {
        escarp_stream_t *out_part = NULL;
        unsigned char out_value = 0;
        escarp_parser_error_t err = ESCARP_PARSER_SUCCESS;
        escarp_stream_error_t serr = ESCARP_STREAM_SUCCESS;

        err = escarp_parse(parser->parsers[i], in, &out_part);
        if (err != ESCARP_PARSER_SUCCESS) {
            free(out_arr);
            return err;
        }

        while ((serr = escarp_stream_read(out_part, &out_value)) ==
               ESCARP_STREAM_SUCCESS) {
            unsigned char *out_arr_tmp = (unsigned char *)realloc(
                out_arr, sizeof(unsigned char) * (out_arr_len + 1));
            if (out_arr_tmp == NULL) {
                escarp_stream_delete(out_part);
                free(out_arr);
                return ESCARP_PARSER_ERROR_MEMORY_ALLOCATION_FAILURE;
            }
            out_arr = out_arr_tmp;
            out_arr[out_arr_len++] = out_value;
        }
        switch (serr) {
        case ESCARP_STREAM_ERROR_NULL_POINTER:
            assert(0);
            break;
        case ESCARP_STREAM_ERROR_MEMORY_ALLOCATION_FAILURE:
            escarp_stream_delete(out_part);
            free(out_arr);
            return ESCARP_PARSER_ERROR_MEMORY_ALLOCATION_FAILURE;
        case ESCARP_STREAM_ERROR_EOS:
            break;
        default:
            assert(0);
            break;
        }

        escarp_stream_delete(out_part);
    }

    out_tmp = escarp_uint8_stream_from(out_arr, out_arr_len);
    if (out_tmp == NULL) {
        free(out_arr);
        return ESCARP_PARSER_ERROR_MEMORY_ALLOCATION_FAILURE;
    }
    free(out_arr);

    *out = out_tmp;
    return ESCARP_PARSER_SUCCESS;
}

static void delete_(escarp_parser_t *base) {
    escarp_sequence_t *parser = (escarp_sequence_t *)base;
    size_t i = 0;

    assert(parser != NULL);

    for (i = 0; i < parser->parsers_len; i++) {
        escarp_parser_delete(parser->parsers[i]);
    }
    free(parser->parsers);
    free(parser);
}

escarp_parser_t *escarp_sequence(size_t len, ...) {
    static escarp_parser_vtable_t vtbl = {parse, delete_};

    va_list args;
    escarp_sequence_t *parser = NULL;
    escarp_parser_t **parsers = NULL;
    size_t contains_null = 0;
    size_t i = 0;

    if (len == 0) {
        return NULL;
    }

    parsers = (escarp_parser_t **)malloc(sizeof(escarp_parser_t *) * len);
    if (parsers == NULL) {
        return NULL;
    }

    va_start(args, len);
    for (i = 0; i < len; i++) {
        if ((parsers[i] = va_arg(args, escarp_parser_t *)) == NULL) {
            contains_null++;
        }
    }
    va_end(args);
    if (contains_null) {
        for (i = 0; i < len; i++) {
            escarp_parser_delete(parsers[i]);
        }
        free(parsers);
        return NULL;
    }

    parser = (escarp_sequence_t *)malloc(sizeof(escarp_sequence_t));
    if (parser == NULL) {
        free(parsers);
        return NULL;
    }

    parser->base.vtbl = &vtbl;
    parser->parsers = parsers;
    parser->parsers_len = len;

    return (escarp_parser_t *)parser;
}