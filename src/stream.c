#include <escarp.h>

void escarp_stream_init(escarp_stream_t *stream) {
    assert(stream != NULL);
    stream->history_count = 0;
    stream->history_index = 0;
    stream->checkpoints_count = 0;
}

int escarp_getc(escarp_stream_t *stream) {
    int ret = EOF;

    assert(stream != NULL);

    if (stream->history_index < stream->history_count) {
        /* `stream->history` is not exhausted */
        ret = stream->history[stream->history_index++];

        if (stream->history_index == stream->history_count &&
            stream->checkpoints_count == 0) {
            /* Release `stream->history` */
            stream->history_count = 0;
            stream->history_index = 0;
        }
    } else {
        assert(stream->history_index == stream->history_count);

        ret = stream->getc(stream);

        if (stream->checkpoints_count != 0) {
            /* `stream->checkpoints` is not exhausted and still need
             * `stream->history` */
            stream->history[stream->history_count++] = ret;
            stream->history_index++;
        }
    }

    return ret;
}

int escarp_use_backtracking(escarp_stream_t *stream,
                            int (*callback)(escarp_stream_t *, void *),
                            void *other_data) {
    assert(stream != NULL);
    assert(callback != NULL);

    stream->checkpoints[stream->checkpoints_count++] = stream->history_index;
    if (!callback(stream, other_data)) {
        stream->history_index =
            stream->checkpoints[stream->checkpoints_count - 1];
        return 0; /* false */
    } else {
        stream->checkpoints_count--;
        return 1; /* true */
    }
}