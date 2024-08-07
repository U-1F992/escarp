#include <escarp.h>

#include "test.h"

#include <assert.h>

int main(void) {
    escarp_error_t err = ESCARP_SUCCESS;

    escarp_any_t any_pool[256];
    size_t any_usage = 0;
#define ANY() escarp_any(&any_pool[any_usage++])

    escarp_value_t value_pool[256];
    size_t value_usage = 0;
#define VALUE(v) escarp_value(&value_pool[value_usage++], (v))

    escarp_sequence_t sequence_pool[256];
    size_t sequence_usage = 0;
#define SEQUENCE(p0, p1)                                                       \
    escarp_sequence(&sequence_pool[sequence_usage++], (p0), (p1))

    escarp_repeat_t repeat_pool[256];
    size_t repeat_usage = 0;
#define REPEAT(t, mi, ma)                                                      \
    escarp_repeat(&repeat_pool[repeat_usage++], (t), (mi), (ma))

    escarp_choice_t choice_pool[256];
    size_t choice_usage = 0;
#define CHOICE(p0, p1) escarp_choice(&choice_pool[choice_usage++], (p0), (p1))

    uint8_array_stream_t stream;
    int out[256];

    escarp_parser_t *nxmc2 = SEQUENCE(VALUE(0xAB), REPEAT(ANY(), 10, 10));

    uint8_array_stream_init(
        &stream, (unsigned char[]){0xAB, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 11);
    err = escarp_parse(nxmc2, &(stream.base), out);
    assert(ESCARP_SUCCESS == err);
    assert(0xAB == out[0]);
    assert(1 == out[1]);
    assert(2 == out[2]);
    assert(3 == out[3]);
    assert(4 == out[4]);
    assert(5 == out[5]);
    assert(6 == out[6]);
    assert(7 == out[7]);
    assert(8 == out[8]);
    assert(9 == out[9]);
    assert(10 == out[10]);

    escarp_parser_t *whitespace = VALUE(' ');
    escarp_parser_t *hex_prefix = SEQUENCE(VALUE('0'), VALUE('x'));
    escarp_parser_t *zero_to_eight = CHOICE(
        VALUE('0'),
        CHOICE(VALUE('1'),
               CHOICE(VALUE('2'),
                      CHOICE(VALUE('3'),
                             CHOICE(VALUE('4'),
                                    CHOICE(VALUE('5'),
                                           CHOICE(VALUE('6'),
                                                  CHOICE(VALUE('7'),
                                                         VALUE('8')))))))));
    escarp_parser_t *hex_digit = CHOICE(
        zero_to_eight,
        CHOICE(VALUE('9'),
               CHOICE(VALUE('a'),
                      CHOICE(VALUE('b'),
                             CHOICE(VALUE('c'),
                                    CHOICE(VALUE('d'),
                                           CHOICE(VALUE('e'), VALUE('f'))))))));
    escarp_parser_t *pokecon_button = CHOICE(
        SEQUENCE(hex_prefix, REPEAT(hex_digit, 4, 4)), REPEAT(hex_digit, 1, 4));
    escarp_parser_t *pokecon_stick_axis =
        SEQUENCE(REPEAT(hex_prefix, 0, 1), REPEAT(hex_digit, 1, 2));
    escarp_parser_t *pokecon_stick =
        SEQUENCE(pokecon_stick_axis, SEQUENCE(whitespace, pokecon_stick_axis));
    escarp_parser_t *e_n_d =
        SEQUENCE(VALUE('e'), SEQUENCE(VALUE('n'), VALUE('d')));
    escarp_parser_t *linebreak =
        SEQUENCE(REPEAT(VALUE('\r'), 0, 1), VALUE('\n'));
    escarp_parser_t *pokecon = SEQUENCE(
        CHOICE(SEQUENCE(
                   pokecon_button,
                   SEQUENCE(whitespace,
                            SEQUENCE(zero_to_eight,
                                     REPEAT(SEQUENCE(whitespace, pokecon_stick),
                                            0, 2)))),
               e_n_d),
        linebreak);

    uint8_array_stream_init(&stream, (unsigned char[]){'e', 'n', 'd', '\n'}, 4);
    assert(ESCARP_SUCCESS == escarp_parse(pokecon, &(stream.base), out));

    uint8_array_stream_init(
        &stream,
        (unsigned char[]){'0', 'x', '0', '0', '0', '0', ' ', '0', '\r', '\n'},
        10);
    assert(ESCARP_SUCCESS == escarp_parse(pokecon, &(stream.base), out));

    uint8_array_stream_init(&stream,
                            (unsigned char[]){'3', ' ', '8', ' ', '0', 'x', 'a',
                                              'e', ' ', '0', 'x', '9', ' ', '8',
                                              '0', ' ', '8', '0', '\n'},
                            19);
    assert(ESCARP_SUCCESS == escarp_parse(pokecon, &(stream.base), out));

    uint8_array_stream_init(&stream,
                            (unsigned char[]){'0', 'x', '0', '0', '0', '2', ' ',
                                              '8', ' ', '8', '0', ' ', '8', '0',
                                              '\r', '\n'},
                            16);
    assert(ESCARP_SUCCESS == escarp_parse(pokecon, &(stream.base), out));

    return 0;
}
