#include <escarp.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

typedef struct test_case_sequence_parse_t {
    size_t len;
    escarp_parser_t **parsers;
    escarp_uint8_t *in;
    size_t in_idx;
    size_t in_len;

    escarp_error_t expected_err;
    size_t expected_in_idx;
    escarp_uint8_t *expected_out;
    size_t expected_out_len;
} test_case_sequence_parse_t;

static size_t test_sequence_parse(void) {
    size_t test_failure = 0;

    escarp_parser_t *parsers_0[8] = {escarp_any(), escarp_value(1)};
    escarp_uint8_t in_0[] = {0, 1};
    escarp_uint8_t expected_out_0[] = {0, 1};

    escarp_parser_t *parsers_1[8] = {escarp_any(), escarp_any()};
    escarp_uint8_t in_1[] = {0, 0, 0};
    escarp_uint8_t expected_out_1[] = {0, 0};

    escarp_parser_t *parsers_2[8] = {escarp_any(), escarp_any()};
    escarp_uint8_t in_2[] = {0};

    test_case_sequence_parse_t test_case[] = {
        {2, parsers_0, in_0, 0, sizeof(in_0), ESCARP_SUCCESS, 2, expected_out_0,
         sizeof(expected_out_0)},
        {2, parsers_1, in_1, 0, sizeof(in_1), ESCARP_SUCCESS, 2, expected_out_1,
         sizeof(expected_out_1)},
        {2, parsers_2, in_2, 0, sizeof(in_2),
         ESCARP_PARSER_ERROR_UNEXPECTED_EOF}};
    size_t test_case_len =
        sizeof(test_case) / sizeof(test_case_sequence_parse_t);
    size_t i = 0;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_parser_t *parser = escarp_sequence(
            test_case[i].len, test_case[i].parsers[0], test_case[i].parsers[1],
            test_case[i].parsers[2], test_case[i].parsers[3],
            test_case[i].parsers[4], test_case[i].parsers[5],
            test_case[i].parsers[6], test_case[i].parsers[7]);
        escarp_error_t actual_err = ESCARP_SUCCESS;
        size_t actual_in_idx = test_case[i].in_idx;
        escarp_uint8_t *actual_out = NULL;
        size_t actual_out_len = 0;
        size_t out_failure = 0;
        size_t j = 0;

        assert(parser != NULL);

        actual_err = escarp_parse(parser, test_case[i].in, &actual_in_idx,
                                  test_case[i].in_len, (void **)&actual_out,
                                  &actual_out_len);

        if (test_case[i].expected_err != actual_err) {
            fprintf(stderr,
                    "index: %d\n"
                    "  expected_err: %d\n"
                    "  actual_err: %d\n",
                    i, test_case[i].expected_err, actual_err);
            test_failure++;
            if (actual_err == ESCARP_SUCCESS) {
                free(actual_out);
            }
            continue;
        }
        if (actual_err != ESCARP_SUCCESS) {
            continue;
        }

        if (test_case[i].expected_in_idx != actual_in_idx) {
            fprintf(stderr,
                    "index: %d\n"
                    "  expected_in_idx: %d\n"
                    "  actual_in_idx: %d\n",
                    i, test_case[i].expected_in_idx, actual_in_idx);
            test_failure++;
            free(actual_out);
            continue;
        }

        if (test_case[i].expected_out_len != actual_out_len) {
            fprintf(stderr,
                    "index: %d\n"
                    "  expected_out_len: %d\n"
                    "  actual_out_len: %d\n",
                    i, test_case[i].expected_out_len, actual_out_len);
            test_failure++;
            free(actual_out);
            continue;
        }

        for (j = 0; j < actual_out_len; j++) {
            if (test_case[i].expected_out[j] != actual_out[j]) {
                fprintf(stderr,
                        "index: %d\n"
                        "  expected_out[%d]: %d\n"
                        "  actual_out[%d]: %d\n",
                        i, j, test_case[i].expected_out[j], j, actual_out[j]);
                out_failure++;
            }
        }
        if (out_failure) {
            test_failure++;
            free(actual_out);
            continue;
        }

        free(actual_out);
        escarp_parser_delete(parser);
    }

    return test_failure;
}