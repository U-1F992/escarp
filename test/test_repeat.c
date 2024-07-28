#include <escarp.h>

#include "test.h"

#include <assert.h>

int main(void) {
    escarp_error_t err = ESCARP_SUCCESS;

    escarp_any_t any[256];
    size_t any_usage = 0;

    escarp_value_t value[256];
    size_t value_usage = 0;

    escarp_sequence_t sequence[256];
    size_t sequence_usage = 0;

    escarp_repeat_t repeat[256];
    size_t repeat_usage = 0;

    uint8_array_stream_t stream;
    int out[256];

    escarp_parser_t *parser = NULL;

    parser = escarp_repeat(NULL, NULL, 1, 0);
    assert(NULL == parser);
    parser = escarp_repeat(&repeat[repeat_usage++], NULL, 1, 0);
    assert(NULL == parser);
    parser = escarp_repeat(&repeat[repeat_usage++],
                           escarp_value(&value[value_usage++], 0x80), 1, 0);
    assert(NULL == parser);

    parser = escarp_repeat(&repeat[repeat_usage++],
                           escarp_value(&value[value_usage++], 0x80), 2, 3);
    assert(NULL != parser);

    uint8_array_stream_init(&stream, (unsigned char[]){}, 0);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);

    uint8_array_stream_init(&stream, (unsigned char[]){0}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_VALUE == err);

    uint8_array_stream_init(&stream, (unsigned char[]){0x80}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);

    uint8_array_stream_init(&stream, (unsigned char[]){0x80, 0x80}, 2);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x80 == out[1]);
    assert(EOF == out[2]);

    uint8_array_stream_init(&stream, (unsigned char[]){0x80, 0x80, 0x80}, 3);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x80 == out[1]);
    assert(0x80 == out[2]);
    assert(EOF == out[3]);

    uint8_array_stream_init(&stream, (unsigned char[]){0x80, 0x80, 0x7F}, 3);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x80 == out[1]);
    assert(EOF == out[2]);
    assert(0x7F == escarp_getc(&(stream.base)));

    uint8_array_stream_init(&stream, (unsigned char[]){0x80, 0x80, 0x80, 0x80},
                            4);
    err = escarp_parse(parser, &(stream.base), out);
    assert(0x80 == out[0]);
    assert(0x80 == out[1]);
    assert(0x80 == out[2]);
    assert(EOF == out[3]);
    assert(0x80 == escarp_getc(&(stream.base)));

    parser = escarp_repeat(
        &repeat[repeat_usage++],
        escarp_sequence(&sequence[sequence_usage++],
                        escarp_any(&any[any_usage++]),
                        escarp_value(&value[value_usage++], 0x80)),
        2, 3);
    assert(NULL != parser);
    uint8_array_stream_init(
        &stream, (unsigned char[]){0x0, 0x80, 0x1, 0x80, 0x2, 0x80}, 6);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x0 == out[0]);
    assert(0x80 == out[1]);
    assert(0x1 == out[2]);
    assert(0x80 == out[3]);
    assert(0x2 == out[4]);
    assert(0x80 == out[5]);
    assert(EOF == out[6]);

    parser = escarp_repeat(&repeat[repeat_usage++],
                           escarp_value(&value[value_usage++], 0xFF), 0, 1);
    assert(NULL != parser);
    uint8_array_stream_init(&stream, (unsigned char[]){0x0}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(EOF == out[0]);

    return 0;
}