#include <escarp.h>

#include <stdlib.h>

escarp_parser_error_t escarp_parse(escarp_parser_t *parser, escarp_stream_t *in,
                                   escarp_stream_t **out) {
    if (parser == NULL || in == NULL || out == NULL) {
        return ESCARP_PARSER_ERROR_NULL_POINTER;
    } else {
        return parser->vtbl->parse(parser, in, out);
    }
}

void escarp_parser_delete(escarp_parser_t *parser) {
    if (parser == NULL) {
        return;
    } else {
        parser->vtbl->delete_(parser);
    }
}