#include <escarp.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct escarp_uint8_stream_t {
    escarp_stream_t base;
    unsigned char *origin;
    size_t idx;
    size_t len;
} escarp_uint8_stream_t;

static escarp_stream_error_t escarp_uint8_stream_read(escarp_stream_t *base,
                                                      void *out) {
    escarp_uint8_stream_t *stream = (escarp_uint8_stream_t *)base;
    unsigned char out_buf = 0;

    assert(stream != NULL);

    if (stream->idx >= stream->len) {
        return ESCARP_STREAM_ERROR_EOS;
    }

    out_buf = stream->origin[stream->idx++];

    *(unsigned char *)out = out_buf;
    return ESCARP_STREAM_SUCCESS;
}

static void escarp_uint8_stream_delete(escarp_stream_t *base) {
    escarp_uint8_stream_t *stream = (escarp_uint8_stream_t *)base;

    assert(stream != NULL);

    free(stream->origin);
    free(stream);
}

escarp_stream_t *escarp_uint8_stream_from(unsigned char *origin, size_t len) {
    static escarp_stream_vtable_t vtbl = {escarp_uint8_stream_read,
                                          escarp_uint8_stream_delete};

    escarp_uint8_stream_t *stream = NULL;
    unsigned char *origin_tmp = NULL;

    if (origin == NULL) {
        return NULL;
    }

    stream = (escarp_uint8_stream_t *)malloc(sizeof(escarp_uint8_stream_t));
    if (stream == NULL) {
        return NULL;
    }

    origin_tmp = (unsigned char *)malloc(sizeof(unsigned char) * len);
    if (origin_tmp == NULL) {
        free(stream);
        return NULL;
    }

    stream->base.vtbl = &vtbl;
    if (escarp_stream_init(&stream->base, sizeof(unsigned char)) !=
        ESCARP_STREAM_SUCCESS) {
        free(origin_tmp);
        free(stream);
        return NULL;
    }

    memcpy(origin_tmp, origin, sizeof(unsigned char) * len);
    stream->origin = origin_tmp;
    stream->idx = 0;
    stream->len = len;

    return (escarp_stream_t *)stream;
}