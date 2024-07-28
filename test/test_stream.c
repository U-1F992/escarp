#include <escarp.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct uint8_array_stream_t {
    escarp_stream_t base;
    unsigned char array[256];
    size_t count;
    size_t index;
} uint8_array_stream_t;

int uint8_array_stream_getc(escarp_stream_t *base) {
    uint8_array_stream_t *stream = (uint8_array_stream_t *)base;
    assert(stream != NULL);
    return stream->index < stream->count ? stream->array[stream->index++] : EOF;
}

void uint8_array_stream_init(uint8_array_stream_t *stream, unsigned char *array,
                             size_t count) {
    assert(stream != NULL);
    assert(array != NULL);

    escarp_stream_init(&(stream->base));
    stream->base.getc = uint8_array_stream_getc;

    stream->count = count > 256 ? 256 : count;
    memcpy(stream->array, array, stream->count);
    stream->index = 0;
}

int test_callback_0(escarp_stream_t *stream) {
    assert(stream != NULL);
    assert(0 == escarp_getc(stream));
    assert(1 == escarp_getc(stream));
    return 0; /* false */
}
int test_callback_1(escarp_stream_t *stream) {
    assert(stream != NULL);
    assert(3 == escarp_getc(stream));
    assert(4 == escarp_getc(stream));
    return 1; /* true */
}

int test_callback_3(escarp_stream_t *);
int test_callback_4(escarp_stream_t *);
int test_callback_2(escarp_stream_t *stream) {
    assert(stream != NULL);
    assert(0 == escarp_getc(stream));
    escarp_use_backtracking(stream, test_callback_3);
    assert(1 == escarp_getc(stream));
    escarp_use_backtracking(stream, test_callback_4);
    assert(4 == escarp_getc(stream));
    return 1;
}
int test_callback_3(escarp_stream_t *stream) {
    assert(stream != NULL);
    assert(1 == escarp_getc(stream));
    assert(2 == escarp_getc(stream));
    return 0;
}
int test_callback_4(escarp_stream_t *stream) {
    assert(stream != NULL);
    assert(2 == escarp_getc(stream));
    assert(3 == escarp_getc(stream));
    return 1;
}
int test_callback_5(escarp_stream_t *stream) {
    assert(stream != NULL);
    assert(5 == escarp_getc(stream));
    assert(EOF == escarp_getc(stream));
    return 0;
}

int main(void) {
    uint8_array_stream_t stream;

    uint8_array_stream_init(&stream, (unsigned char[]){0, 1}, 2);

    assert(0 == escarp_getc(&(stream.base)));
    assert(1 == escarp_getc(&(stream.base)));
    assert(EOF == escarp_getc(&(stream.base)));

    uint8_array_stream_init(&stream, (unsigned char[]){0, 1, 2, 3, 4, 5}, 6);
    escarp_use_backtracking(&(stream.base), test_callback_0);
    assert(0 == escarp_getc(&(stream.base)));
    assert(1 == escarp_getc(&(stream.base)));
    assert(2 == escarp_getc(&(stream.base)));
    escarp_use_backtracking(&(stream.base), test_callback_1);
    assert(5 == escarp_getc(&(stream.base)));
    assert(EOF == escarp_getc(&(stream.base)));

    uint8_array_stream_init(&stream, (unsigned char[]){0, 1, 2, 3, 4, 5}, 6);
    escarp_use_backtracking(&(stream.base), test_callback_2);
    escarp_use_backtracking(&(stream.base), test_callback_5);
    assert(5 == escarp_getc(&(stream.base)));
    assert(EOF == escarp_getc(&(stream.base)));

    return 0;
}