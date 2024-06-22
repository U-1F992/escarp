#include <escarp.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* any */

static size_t test_any_new(void) {
    size_t test_failure = 0;
    escarp_parser_t *parser = NULL;

    printf("* %s\n", __func__);

    parser = escarp_any();
    if (parser == NULL) {
        fprintf(stderr,
                "index: %d\n"
                "  expected: %s\n"
                "  actual: %s\n",
                0, "(Non-NULL)", "NULL");
        test_failure++;
    }
    escarp_parser_delete(parser);

    return test_failure;
}

/* value */

typedef struct test_case_value_new_t {
    unsigned char value;
} test_case_value_new_t;

static size_t test_value_new(void) {
    size_t test_failure = 0;

    test_case_value_new_t test_case[] = {{0}, {128}, {255}};
    size_t test_case_len = sizeof(test_case) / sizeof(test_case_value_new_t);
    size_t i = 0;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_parser_t *parser = escarp_value(test_case[i].value);
        if (parser == NULL) {
            fprintf(stderr,
                    "index: %d\n"
                    "  expected: %s\n"
                    "  actual: %s\n",
                    i, "(Non-NULL)", "NULL");
            test_failure++;
        }
        escarp_parser_delete(parser);
    }

    return test_failure;
}

/* sequence */

typedef struct test_case_sequence_new_t {
    size_t len;
    escarp_parser_t **parsers;

    int expected_null;
} test_case_sequence_new_t;

static size_t test_sequence_new(void) {
    size_t test_failure = 0;

    escarp_parser_t *parsers_0[8] = {};
    escarp_parser_t *parsers_1[8] = {escarp_any()};
    escarp_parser_t *parsers_2[8] = {escarp_any(), escarp_value(0)};
    escarp_parser_t *parsers_3[8] = {NULL, escarp_value(0)};
    escarp_parser_t *parsers_4[8] = {escarp_any(), escarp_any(), escarp_any(),
                                     escarp_any()};
    test_case_sequence_new_t test_case[] = {{0, parsers_0, 1},
                                            {1, parsers_1, 0},
                                            {2, parsers_2, 0},
                                            {2, parsers_3, 1},
                                            {4, parsers_4, 0}};
    size_t test_case_len = sizeof(test_case) / sizeof(test_case_sequence_new_t);
    size_t i = 0;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_parser_t *parser = escarp_sequence(
            test_case[i].len, test_case[i].parsers[0], test_case[i].parsers[1],
            test_case[i].parsers[2], test_case[i].parsers[3],
            test_case[i].parsers[4], test_case[i].parsers[5],
            test_case[i].parsers[6], test_case[i].parsers[7]);
        if (test_case[i].expected_null /**/
                ? parser != NULL
                : parser == NULL) {
            fprintf(stderr,
                    "index: %d\n"
                    "  expected: %s\n"
                    "  actual: %s\n",
                    i, test_case[i].expected_null ? "NULL" : "(Non-NULL)",
                    test_case[i].expected_null ? "(Non-NULL)" : "NULL");
            test_failure++;
        }
        escarp_parser_delete(parser);
    }

    return test_failure;
}

typedef struct test_case_parse_t {
    escarp_parser_t *parser;
    escarp_stream_t *in;

    escarp_parser_error_t expected_err;
    escarp_stream_t *expected_out;
} test_case_parse_t;

static size_t test_parse(void) {
    size_t test_failure = 0;

    unsigned char in_0[] = {0};
    unsigned char out_0[] = {0};

    unsigned char in_1[] = {128};
    unsigned char out_1[] = {128};

    unsigned char in_2[] = {};

    unsigned char in_3[] = {0, 1};
    unsigned char out_3[] = {0};

    unsigned char in_4[] = {1};
    unsigned char out_4[] = {1};

    unsigned char in_5[] = {0};

    unsigned char in_6[] = {};

    unsigned char in_7[] = {1, 2};
    unsigned char out_7[] = {1};

    unsigned char in_8[] = {128, 1};
    unsigned char out_8[] = {128, 1};

    unsigned char in_9[] = {255, 1};
    unsigned char out_9[] = {255, 1};

    unsigned char in_10[] = {128, 2};

    test_case_parse_t test_case[] = {
        {escarp_any(),                                 /**/
         escarp_uint8_stream_from(in_0, sizeof(in_0)), /**/
         ESCARP_PARSER_SUCCESS,                        /**/
         escarp_uint8_stream_from(out_0, sizeof(out_0))},
        {escarp_any(),                                 /**/
         escarp_uint8_stream_from(in_1, sizeof(in_1)), /**/
         ESCARP_PARSER_SUCCESS,                        /**/
         escarp_uint8_stream_from(out_1, sizeof(out_1))},
        {escarp_any(),                                 /**/
         escarp_uint8_stream_from(in_2, sizeof(in_2)), /**/
         ESCARP_PARSER_ERROR_UNEXPECTED_EOS,           /**/
         NULL},
        {escarp_any(),                                 /**/
         escarp_uint8_stream_from(in_3, sizeof(in_3)), /**/
         ESCARP_PARSER_SUCCESS,                        /**/
         escarp_uint8_stream_from(out_3, sizeof(out_3))},
        /*****/
        /*****/
        {escarp_value(1),                              /**/
         escarp_uint8_stream_from(in_4, sizeof(in_4)), /**/
         ESCARP_PARSER_SUCCESS,                        /**/
         escarp_uint8_stream_from(out_4, sizeof(out_4))},
        {escarp_value(1),                              /**/
         escarp_uint8_stream_from(in_5, sizeof(in_5)), /**/
         ESCARP_PARSER_ERROR_UNEXPECTED_VALUE,         /**/
         NULL},
        {escarp_value(1),                              /**/
         escarp_uint8_stream_from(in_6, sizeof(in_6)), /**/
         ESCARP_PARSER_ERROR_UNEXPECTED_EOS,           /**/
         NULL},
        {escarp_value(1),                              /**/
         escarp_uint8_stream_from(in_7, sizeof(in_7)), /**/
         ESCARP_PARSER_SUCCESS,                        /**/
         escarp_uint8_stream_from(out_7, sizeof(out_7))},
        /*****/
        /*****/
        {escarp_sequence(2, escarp_any(), escarp_value(1)), /**/
         escarp_uint8_stream_from(in_8, sizeof(in_8)),      /**/
         ESCARP_PARSER_SUCCESS,                             /**/
         escarp_uint8_stream_from(out_8, sizeof(out_8))},
        {escarp_sequence(2, escarp_any(), escarp_value(1)), /**/
         escarp_uint8_stream_from(in_9, sizeof(in_9)),      /**/
         ESCARP_PARSER_SUCCESS,                             /**/
         escarp_uint8_stream_from(out_9, sizeof(out_9))},
        {escarp_sequence(2, escarp_any(), escarp_value(1)), /**/
         escarp_uint8_stream_from(in_10, sizeof(in_10)),    /**/
         ESCARP_PARSER_ERROR_UNEXPECTED_VALUE,              /**/
         NULL}};
    size_t test_case_len = sizeof(test_case) / sizeof(test_case_parse_t);
    size_t i = 0;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_parser_error_t actual_err = ESCARP_PARSER_SUCCESS;
        escarp_stream_t *actual_out = NULL;
        size_t out_failure = 0;

        actual_err =
            escarp_parse(test_case[i].parser, test_case[i].in, &actual_out);

        if (test_case[i].expected_err != actual_err) {
            fprintf(stderr,
                    "index: %d\n"
                    "  expected_err: %d\n"
                    "  actual_err: %d\n",
                    i, test_case[i].expected_err, actual_err);
            test_failure++;
            if (actual_err == ESCARP_PARSER_SUCCESS) {
                escarp_stream_delete(actual_out);
            }
            continue;
        }
        if (actual_err != ESCARP_PARSER_SUCCESS) {
            continue;
        }

        while (1) {
            unsigned char expected_out_value = 0;
            unsigned char actual_out_value = 0;
            escarp_stream_error_t expected_out_err = escarp_stream_read(
                test_case[i].expected_out, &expected_out_value);
            escarp_stream_error_t actual_out_err =
                escarp_stream_read(actual_out, &actual_out_value);

            if (expected_out_err != actual_out_err) {
                fprintf(stderr,
                        "index: %d\n"
                        "  expected_out_err: %d\n"
                        "  actual_out_err: %d\n",
                        i, expected_out_err, actual_out_err);
                out_failure++;
            }
            if (expected_out_err == ESCARP_STREAM_ERROR_EOS ||
                actual_out_err == ESCARP_STREAM_ERROR_EOS) {
                break;
            }

            if (expected_out_value != actual_out_value) {
                fprintf(stderr,
                        "index: %d\n"
                        "  expected_out_value: %d\n"
                        "  actual_out_value: %d\n",
                        i, expected_out_value, actual_out_value);
                out_failure++;
            }
        }
        if (out_failure) {
            test_failure++;
            escarp_stream_delete(actual_out);
            continue;
        }

        escarp_stream_delete(actual_out);
    }

    return test_failure;
}
