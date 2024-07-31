#ifndef ESCARP_TEST_H
#define ESCARP_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <escarp.h>

#include <assert.h>
#include <string.h>

typedef struct uint8_array_stream_t {
    escarp_stream_t base;
    unsigned char array[256];
    size_t count;
    size_t index;
} uint8_array_stream_t;

static int uint8_array_stream_getc(escarp_stream_t *base) {
    uint8_array_stream_t *stream = (uint8_array_stream_t *)base;
    assert(stream != NULL);
    return stream->index < stream->count ? stream->array[stream->index++] : EOF;
}

static int history_buffer[32];
static size_t checkpoints_buffer[32];

static void uint8_array_stream_init(uint8_array_stream_t *stream,
                                    unsigned char *array, size_t count) {
    assert(stream != NULL);
    assert(array != NULL);

    escarp_stream_init(&(stream->base), history_buffer, 32, checkpoints_buffer,
                       32);
    stream->base.getc = uint8_array_stream_getc;

    stream->count = count > 256 ? 256 : count;
    memcpy(stream->array, array, stream->count);
    stream->index = 0;
}

#ifdef __cplusplus
}
#endif

#endif /* ESCARP_TEST_H */