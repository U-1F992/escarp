#include <escarp.h>

escarp_error_t escarp_parse(escarp_parser_t *parser, escarp_uint8_t *in, size_t *in_idx, size_t in_len, void **out, size_t *out_len) {
    if (parser == NULL || in == NULL || in_idx == NULL || out == NULL || out_len == NULL) {
        return ESCARP_ERROR_NULL_POINTER;
    } else {
        return parser->vtbl->parse(parser, in, in_idx, in_len, out, out_len);
    }
}

void escarp_parser_delete(escarp_parser_t *parser) {
    if (parser == NULL) {
        return;
    } else {
        parser->vtbl->delete_(parser);
    }
}