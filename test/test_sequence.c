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

    uint8_array_stream_t stream;
    int out[256];

    escarp_parser_t *parser = NULL;

    parser = escarp_sequence(NULL, NULL, NULL);
    assert(NULL == parser);
    parser = escarp_sequence(&sequence[sequence_usage++], NULL, NULL);
    assert(NULL == parser);
    parser = escarp_sequence(&sequence[sequence_usage++],
                             escarp_any(&any[any_usage++]), NULL);
    assert(NULL == parser);

    parser = escarp_sequence(&sequence[sequence_usage++],
                             escarp_any(&any[any_usage++]),
                             escarp_value(&value[value_usage++], 0xFF));
    assert(NULL != parser);

    uint8_array_stream_init(&stream, (unsigned char[]){}, 0);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);

    uint8_array_stream_init(&stream, (unsigned char[]){0}, 1);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);

    uint8_array_stream_init(&stream, (unsigned char[]){0xFE, 0xFF}, 2);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0xFE == out[0]);
    assert(0xFF == out[1]);
    assert(EOF == out[2]);

    uint8_array_stream_init(&stream, (unsigned char[]){0x0, 0xFF}, 2);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x0 == out[0]);
    assert(0xFF == out[1]);
    assert(EOF == out[2]);

    parser = escarp_sequence(
        &sequence[sequence_usage++],                 /**/
        escarp_any(&any[any_usage++]),               /**/
        escarp_sequence(&sequence[sequence_usage++], /**/
                        escarp_any(&any[any_usage++]),
                        escarp_value(&value[value_usage++], 0xFF)));
    assert(NULL != parser);

    uint8_array_stream_init(&stream, (unsigned char[]){0x0, 0x1, 0xFF}, 3);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x0 == out[0]);
    assert(0x1 == out[1]);
    assert(0xFF == out[2]);
    assert(EOF == out[3]);

    return 0;
}