#include <escarp.h>

#include "test.h"

#include <assert.h>

int main(void) {
    escarp_error_t err = ESCARP_SUCCESS;

    escarp_value_t value[256];
    size_t value_usage = 0;

    uint8_array_stream_t stream;
    int out[256];

    escarp_parser_t *parser = NULL;

    parser = escarp_value(NULL, 0);
    assert(NULL == parser);

    parser = escarp_value(&value[value_usage++], 0);
    assert(NULL != parser);

    uint8_array_stream_init(&stream, (unsigned char[]){}, 0);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);

    uint8_array_stream_init(&stream, (unsigned char[]){0}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0 == out[0]);
    assert(EOF == out[1]);

    uint8_array_stream_init(&stream, (unsigned char[]){1}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_VALUE == err);

    uint8_array_stream_init(&stream, (unsigned char[]){0, 1}, 2);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0 == out[0]);
    assert(EOF == out[1]);

    parser = escarp_value(&value[value_usage++], 0x80);
    assert(NULL != parser);
    uint8_array_stream_init(&stream, (unsigned char[]){0x80}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(EOF == out[1]);

    return 0;
}
