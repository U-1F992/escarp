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

static escarp_error_t parse(escarp_parser_t *base, escarp_uint8_t *in,
                            size_t *in_idx, size_t in_len, void **out,
                            size_t *out_len) {
    escarp_sequence_t *parser = (escarp_sequence_t *)base;
    void *out_tmp = NULL;
    size_t out_tmp_len = 0;
    escarp_uint8_t *out_buf = NULL;
    size_t out_buf_len = 0;
    escarp_uint8_t *out_buf_tmp = NULL;
    escarp_error_t err = ESCARP_SUCCESS;
    size_t i = 0;

    assert(parser != NULL);
    assert(in != NULL);
    assert(in_idx != NULL);
    assert(out != NULL);
    assert(out_len != NULL);

    for (i = 0; i < parser->parsers_len; i++) {
        err = escarp_parse(parser->parsers[i], in, in_idx, in_len, &out_tmp,
                           &out_tmp_len);
        if (err != ESCARP_SUCCESS) {
            free(out_buf);
            return err;
        }

        out_buf_tmp = (escarp_uint8_t *)realloc(
            out_buf, sizeof(escarp_uint8_t) * (out_buf_len + out_tmp_len));
        if (out_buf_tmp == NULL) {
            free(out_tmp);
            free(out_buf);
            return ESCARP_ERROR_MEMORY_ALLOCATION_FAILURE;
        }
        out_buf = out_buf_tmp;

        memcpy(out_buf + out_buf_len, out_tmp, out_tmp_len);
        out_buf_len += out_tmp_len;

        free(out_tmp);
    }

    *out = (void *)out_buf;
    *out_len = out_buf_len;
    return ESCARP_SUCCESS;
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
    static escarp_parser_t base = {&vtbl};

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

    parser->base = base;
    parser->parsers = parsers;
    parser->parsers_len = len;

    return (escarp_parser_t *)parser;
}