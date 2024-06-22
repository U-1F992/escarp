#include <escarp.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

escarp_stream_error_t escarp_stream_init(escarp_stream_t *stream,
                                         size_t element_size) {
    if (stream == NULL) {
        return ESCARP_STREAM_ERROR_NULL_POINTER;
    }

    stream->element_size = element_size;

    stream->history = NULL;
    stream->history_len = 0;
    stream->history_idx = 0;

    stream->checkpoints = NULL;
    stream->checkpoints_len = 0;

    return ESCARP_STREAM_SUCCESS;
}

void escarp_stream_delete(escarp_stream_t *stream) {
    if (stream == NULL) {
        return;
    } else {
        size_t i = 0;

        free(stream->history);
        stream->history = NULL;
        stream->history_len = 0;
        stream->history_idx = 0;

        free(stream->checkpoints);
        stream->checkpoints = NULL;
        stream->checkpoints_len = 0;

        stream->vtbl->delete_(stream);
    }
}

escarp_stream_error_t escarp_stream_read(escarp_stream_t *stream, void *out) {
    void *out_buf = NULL;
    size_t i = 0;

    if (stream == NULL || out == NULL) {
        return ESCARP_STREAM_ERROR_NULL_POINTER;
    }

    out_buf = malloc(stream->element_size);
    if (out_buf == NULL) {
        return ESCARP_STREAM_ERROR_MEMORY_ALLOCATION_FAILURE;
    }

    if (stream->history_idx < stream->history_len) {
        /* `stream->history` is not exhausted */
        memcpy(out_buf,
               stream->history + stream->element_size * stream->history_idx,
               stream->element_size);
        stream->history_idx++;

        if (stream->history_idx == stream->history_len &&
            stream->checkpoints_len == 0) {
            free(stream->history);
            stream->history = NULL;
            stream->history_len = 0;
            stream->history_idx = 0;
        }
    } else {
        escarp_stream_error_t err = stream->vtbl->read(stream, out_buf);
        if (err != ESCARP_STREAM_SUCCESS) {
            free(out_buf);
            return err;
        }

        if (stream->checkpoints_len != 0) {
            /* `stream->checkpoints` is not exhausted and still need
             * `stream->history` */
            void *history_tmp =
                realloc(stream->history,
                        stream->element_size * (stream->history_len + 1));
            if (history_tmp == NULL) {
                free(out_buf);
                return ESCARP_STREAM_ERROR_MEMORY_ALLOCATION_FAILURE;
            }
            stream->history = history_tmp;
            memcpy(stream->history + stream->element_size * stream->history_len,
                   out_buf, stream->element_size);
            stream->history_len++;
            stream->history_idx++;
        }
    }

    memcpy(out, out_buf, stream->element_size);
    free(out_buf);
    return ESCARP_STREAM_SUCCESS;
}

escarp_stream_error_t
escarp_stream_begin_transaction(escarp_stream_t *stream,
                                escarp_stream_transaction_t transaction,
                                void *other_data) {
    size_t *checkpoints_tmp = NULL;

    if (stream == NULL || transaction == NULL) {
        return ESCARP_STREAM_ERROR_NULL_POINTER;
    }

    checkpoints_tmp = (size_t *)realloc(
        stream->checkpoints, sizeof(size_t) * (stream->checkpoints_len + 1));
    if (checkpoints_tmp == NULL) {
        return ESCARP_STREAM_ERROR_MEMORY_ALLOCATION_FAILURE;
    }
    stream->checkpoints = checkpoints_tmp;
    stream->checkpoints[stream->checkpoints_len++] = stream->history_idx;

    if (!transaction(stream, other_data)) {
        /* Rewind to the checkpoint */
        if (stream->checkpoints_len == 0) {
            assert(0);
        } else {
            stream->history_idx =
                stream->checkpoints[stream->checkpoints_len - 1];
        }
    }
    if (stream->checkpoints_len == 0) {
        assert(0);
    } else if (stream->checkpoints_len - 1 == 0) {
        /*
         * > When the requested size is 0, the behavior of the memory
         * allocation functions `malloc()`, `calloc()`, and `realloc()`
         * is implementation-defined.
         * https://wiki.sei.cmu.edu/confluence/display/c/MEM04-C.+Beware+of+zero-length+allocations
         */
        free(stream->checkpoints);
        stream->checkpoints = NULL;
        stream->checkpoints_len = 0;
    } else {
        checkpoints_tmp =
            (size_t *)realloc(stream->checkpoints,
                              sizeof(size_t) * (stream->checkpoints_len - 1));
        if (checkpoints_tmp == NULL) {
            return ESCARP_STREAM_ERROR_MEMORY_ALLOCATION_FAILURE;
        }
        stream->checkpoints = checkpoints_tmp;
        stream->checkpoints_len--;
    }

    return ESCARP_STREAM_SUCCESS;
}