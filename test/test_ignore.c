#include <escarp.h>

#include "test.h"

#include <assert.h>
#include <limits.h>

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

    escarp_ignore_t ignore[256];
    size_t ignore_usage = 0;

    uint8_array_stream_t stream;
    int out[256];

    escarp_parser_t *parser = NULL;

    parser = escarp_ignore(&ignore[ignore_usage++], NULL);
    assert(NULL == parser);

    parser =
        escarp_ignore(&ignore[ignore_usage++], escarp_any(&any[any_usage++]));
    assert(NULL != parser);

    uint8_array_stream_init(&stream, (unsigned char[]){}, 0);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_ERROR_UNEXPECTED_EOF == err);

    parser = escarp_sequence(
        &sequence[sequence_usage++], escarp_value(&value[value_usage++], 0x80),
        escarp_sequence(
            &sequence[sequence_usage++],
            escarp_ignore(
                &ignore[ignore_usage++],
                escarp_repeat(&repeat[repeat_usage++],
                              escarp_value(&value[value_usage++], 0xFF), 0,
                              SIZE_MAX)),
            escarp_value(&value[value_usage++], 0x81)));
    assert(NULL != parser);

    uint8_array_stream_init(&stream, (unsigned char[]){0x80, 0x81}, 2);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x81 == out[1]);
    assert(EOF == out[2]);

    uint8_array_stream_init(&stream, (unsigned char[]){0x80, 0xFF, 0x81}, 3);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x81 == out[1]);
    assert(EOF == out[2]);

    uint8_array_stream_init(&stream, (unsigned char[]){0x80, 0xFF, 0xFF, 0x81},
                            4);
    err = escarp_parse(parser, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0x80 == out[0]);
    assert(0x81 == out[1]);
    assert(EOF == out[2]);

    return 0;
}
