#include <escarp.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static escarp_stream_transaction_status_t
test_transaction_1(escarp_stream_t *stream, void *other_data) {
    unsigned char out = 0;

    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 2);
    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 3);

    return ESCARP_STREAM_TRANSACTION_COMMIT;
}

static escarp_stream_transaction_status_t
test_transaction_2(escarp_stream_t *stream, void *other_data) {
    unsigned char out = 0;

    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 5);
    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 6);

    return ESCARP_STREAM_TRANSACTION_ROLLBACK;
}

static escarp_stream_transaction_status_t
test_transaction_0(escarp_stream_t *stream, void *other_data) {
    unsigned char out = 0;

    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 1);

    assert(ESCARP_STREAM_SUCCESS ==
           escarp_stream_begin_transaction(stream, test_transaction_1, NULL));
    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 4);

    assert(ESCARP_STREAM_SUCCESS ==
           escarp_stream_begin_transaction(stream, test_transaction_2, NULL));
    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 5);

    return ESCARP_STREAM_TRANSACTION_ROLLBACK;
}

static size_t test_stream(void) {
    unsigned char origin[] = {0, 1, 2, 3, 4, 5, 6};
    escarp_stream_t *stream = escarp_uint8_stream_from(origin, sizeof(origin));
    unsigned char out = 0;

    printf("* %s\n", __func__);

    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 0);

    assert(ESCARP_STREAM_SUCCESS ==
           escarp_stream_begin_transaction(stream, test_transaction_0, NULL));

    assert(ESCARP_STREAM_SUCCESS == escarp_stream_read(stream, &out));
    assert(out == 1);

    escarp_stream_delete(stream);

    return 0;
}