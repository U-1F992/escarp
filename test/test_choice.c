#include <escarp.h>

#include "test.h"

#include <assert.h>

int main(void) {
    escarp_error_t err = ESCARP_SUCCESS;

    escarp_value_t value[256];
    size_t value_usage = 0;

    escarp_sequence_t sequence[256];
    size_t sequence_usage = 0;

    escarp_choice_t choice[256];
    size_t choice_usage = 0;

    uint8_array_stream_t stream;
    int out[256];

    escarp_parser_t *parser = NULL;

    parser = escarp_choice(NULL, NULL, NULL);
    assert(NULL == parser);
    parser = escarp_choice(&choice[choice_usage++], NULL, NULL);
    assert(NULL == parser);
    parser = escarp_choice(&choice[choice_usage++],
                           escarp_value(&value[value_usage++], 0), NULL);
    assert(NULL == parser);

    parser = escarp_choice(&choice[choice_usage++],
                           escarp_value(&value[value_usage++], 0),
                           escarp_value(&value[value_usage++], 0xFF));
    assert(NULL != parser);

    uint8_array_stream_init(&stream, (unsigned char[]){}, 0);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);

    uint8_array_stream_init(&stream, (unsigned char[]){0}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x0 == out[0]);
    assert(EOF == out[1]);

    uint8_array_stream_init(&stream, (unsigned char[]){0xFF}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0xFF == out[0]);
    assert(EOF == out[1]);

    uint8_array_stream_init(&stream, (unsigned char[]){0x80}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_VALUE == err);

    parser = escarp_choice(
        &choice[choice_usage++],
        escarp_sequence(&sequence[sequence_usage++],
                        escarp_value(&value[value_usage++], 'a'),
                        escarp_value(&value[value_usage++], 'b')),
        escarp_sequence(&sequence[sequence_usage++],
                        escarp_value(&value[value_usage++], 'c'),
                        escarp_value(&value[value_usage++], 'd')));
    assert(NULL != parser);

    uint8_array_stream_init(&stream, (unsigned char[]){'a', 'b'}, 2);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert('a' == out[0]);
    assert('b' == out[1]);
    assert(EOF == out[2]);

    uint8_array_stream_init(&stream, (unsigned char[]){'c', 'd'}, 2);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert('c' == out[0]);
    assert('d' == out[1]);
    assert(EOF == out[2]);

    uint8_array_stream_init(&stream, (unsigned char[]){'a', 'd'}, 2);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_VALUE == err);

    return 0;
}