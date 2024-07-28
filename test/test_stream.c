#include <escarp.h>

#include "test.h"

#include <assert.h>

int test_callback_0(escarp_stream_t *stream, void *other_data) {
    assert(stream != NULL);
    assert(0 == escarp_getc(stream));
    assert(1 == escarp_getc(stream));
    return 0; /* false */
}
int test_callback_1(escarp_stream_t *stream, void *other_data) {
    assert(stream != NULL);
    assert(3 == escarp_getc(stream));
    assert(4 == escarp_getc(stream));
    return 1; /* true */
}

int test_callback_3(escarp_stream_t *, void *);
int test_callback_4(escarp_stream_t *, void *);
int test_callback_2(escarp_stream_t *stream, void *other_data) {
    assert(stream != NULL);
    assert(0 == escarp_getc(stream));
    escarp_use_backtracking(stream, test_callback_3, other_data);
    assert(1 == escarp_getc(stream));
    escarp_use_backtracking(stream, test_callback_4, other_data);
    assert(4 == escarp_getc(stream));
    return 1;
}
int test_callback_3(escarp_stream_t *stream, void *other_data) {
    assert(stream != NULL);
    assert(1 == escarp_getc(stream));
    assert(2 == escarp_getc(stream));
    return 0;
}
int test_callback_4(escarp_stream_t *stream, void *other_data) {
    assert(stream != NULL);
    assert(2 == escarp_getc(stream));
    assert(3 == escarp_getc(stream));
    return 1;
}
int test_callback_5(escarp_stream_t *stream, void *other_data) {
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
    assert(0 == escarp_use_backtracking(&(stream.base), test_callback_0, NULL));
    assert(0 == escarp_getc(&(stream.base)));
    assert(1 == escarp_getc(&(stream.base)));
    assert(2 == escarp_getc(&(stream.base)));
    assert(0 != escarp_use_backtracking(&(stream.base), test_callback_1, NULL));
    assert(5 == escarp_getc(&(stream.base)));
    assert(EOF == escarp_getc(&(stream.base)));

    uint8_array_stream_init(&stream, (unsigned char[]){0, 1, 2, 3, 4, 5}, 6);
    escarp_use_backtracking(&(stream.base), test_callback_2, NULL);
    escarp_use_backtracking(&(stream.base), test_callback_5, NULL);
    assert(5 == escarp_getc(&(stream.base)));
    assert(EOF == escarp_getc(&(stream.base)));

    return 0;
}