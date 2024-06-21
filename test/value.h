#include <escarp.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct test_case_value_new_t {
    escarp_uint8_t value;
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

typedef struct test_case_value_parse_t {
    escarp_uint8_t value;
    escarp_uint8_t *in;
    size_t in_idx;
    size_t in_len;

    escarp_error_t expected_err;
    size_t expected_in_idx;
    escarp_uint8_t *expected_out;
    size_t expected_out_len;
} test_case_value_parse_t;

static size_t test_value_parse(void) {
    size_t test_failure = 0;

    escarp_uint8_t in_0[] = {0};
    escarp_uint8_t expected_out_0[] = {0};
    escarp_uint8_t in_1[] = {1};
    escarp_uint8_t in_2[] = {1};
    escarp_uint8_t expected_out_2[] = {1};
    escarp_uint8_t in_3[] = {0};
    test_case_value_parse_t test_case[] = {
        {0, in_0, 0, sizeof(in_0), ESCARP_SUCCESS, 1, expected_out_0,
         sizeof(expected_out_0)},
        {0, in_1, 0, sizeof(in_1), ESCARP_PARSER_ERROR_UNEXPECTED_VALUE},
        {1, in_2, 0, sizeof(in_2), ESCARP_SUCCESS, 1, expected_out_2,
         sizeof(expected_out_2)},
        {1, in_3, 0, sizeof(in_3), ESCARP_PARSER_ERROR_UNEXPECTED_VALUE}};
    size_t test_case_len = sizeof(test_case) / sizeof(test_case_value_parse_t);
    size_t i = 0;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_parser_t *parser = escarp_value(test_case[i].value);
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