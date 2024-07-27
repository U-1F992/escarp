#include <escarp.h>

#include <stdio.h>

#include "parser.h"
#include "stream.h"

typedef struct test_case_array_new_t {
    escarp_array_t **arr;
    size_t type;
    void *data;
    size_t len;

    escarp_array_error_t expected_err;
} test_case_array_new_t;

size_t test_array_new(void) {
    size_t test_failure = 0;

    escarp_array_t *non_null_arr = NULL;
    unsigned char empty_data[] = {};
    unsigned char non_null_data[] = {0};

    test_case_array_new_t test_cases[] = {
        {NULL, 0, NULL, 0, ESCARP_ARRAY_ERROR_NULL_POINTER},
        {NULL, sizeof(unsigned char), NULL, 0, ESCARP_ARRAY_ERROR_NULL_POINTER},
        {NULL, sizeof(unsigned char), empty_data, 0,
         ESCARP_ARRAY_ERROR_NULL_POINTER},
        {&non_null_arr, 0, NULL, 0, ESCARP_ARRAY_ERROR_INVALID_TYPE},
        {&non_null_arr, sizeof(unsigned char), NULL, 0, ESCARP_ARRAY_SUCCESS},
        {&non_null_arr, sizeof(unsigned char), empty_data, 0,
         ESCARP_ARRAY_SUCCESS},
        {&non_null_arr, sizeof(unsigned char), non_null_data,
         sizeof(non_null_data), ESCARP_ARRAY_SUCCESS}};
    size_t test_case_len = sizeof(test_cases) / sizeof(test_case_array_new_t);
    size_t i = 0;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_array_t **arr = test_cases[i].arr;
        size_t type = test_cases[i].type;
        void *data = test_cases[i].data;
        size_t len = test_cases[i].len;
        escarp_array_error_t actual_err;

        actual_err = escarp_array_new(arr, type, data, len);
        if (test_cases[i].expected_err != actual_err) {
            fprintf(stderr, "index: %d, expected_err: %d, actual_err: %d\n", i,
                    test_cases[i].expected_err, actual_err);
            escarp_array_delete(*arr);
            continue;
        }
        if (actual_err != ESCARP_ARRAY_SUCCESS) {
            continue;
        }
        escarp_array_delete(*arr);
    }
    return test_failure;
}

typedef struct test_case_array_at_t {
    escarp_array_t *arr;
    size_t idx;
    void *out;

    escarp_array_error_t expected_err;
    unsigned char expected_out;
} test_case_array_at_t;

size_t test_array_at(void) {
    size_t test_failure = 0;

    escarp_array_t *null_arr = NULL;
    escarp_array_t *zero_size_arr_0 = NULL;
    escarp_array_t *zero_size_arr_1 = NULL;
    escarp_array_t *one_size_arr_0 = NULL;
    escarp_array_t *one_size_arr_1 = NULL;
    unsigned char one_size_arr_[] = {1};

    unsigned char out = 0;

    test_case_array_at_t test_cases[] = {
        {null_arr, 0, NULL, ESCARP_ARRAY_ERROR_NULL_POINTER},
        {zero_size_arr_0, 0, NULL, ESCARP_ARRAY_ERROR_NULL_POINTER},
        {zero_size_arr_1, 0, &out, ESCARP_ARRAY_ERROR_OUT_OF_BOUNDS},
        {one_size_arr_0, 0, &out, ESCARP_ARRAY_SUCCESS, 1},
        {one_size_arr_1, 1, &out, ESCARP_ARRAY_ERROR_OUT_OF_BOUNDS}};
    size_t test_case_len = sizeof(test_cases) / sizeof(test_case_array_at_t);
    size_t i = 0;

    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&zero_size_arr_0, sizeof(unsigned char), NULL, 0));
    test_cases[1].arr = zero_size_arr_0;
    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&zero_size_arr_1, sizeof(unsigned char), NULL, 0));
    test_cases[2].arr = zero_size_arr_1;
    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&one_size_arr_0, sizeof(unsigned char),
                             one_size_arr_, sizeof(one_size_arr_)));
    test_cases[3].arr = one_size_arr_0;
    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&one_size_arr_1, sizeof(unsigned char),
                             one_size_arr_, sizeof(one_size_arr_)));
    test_cases[4].arr = one_size_arr_1;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_array_t *arr = test_cases[i].arr;
        size_t idx = test_cases[i].idx;
        void *out = test_cases[i].out;
        escarp_array_error_t actual_err = ESCARP_ARRAY_SUCCESS;
        unsigned char actual_out = 0;

        actual_err = escarp_array_at(arr, idx, out);
        if (test_cases[i].expected_err != actual_err) {
            fprintf(stderr, "index: %d, expected_err: %d, actual_err: %d\n", i,
                    test_cases[i].expected_err, actual_err);
            escarp_array_delete(arr);
            continue;
        }
        if (actual_err != ESCARP_ARRAY_SUCCESS) {
            escarp_array_delete(arr);
            continue;
        }

        actual_out = *(unsigned char *)out;
        if (test_cases[i].expected_out != actual_out) {
            fprintf(stderr, "index: %d, expected_out: %d, actual_out: %d\n", i,
                    test_cases[i].expected_out, actual_out);
            escarp_array_delete(arr);
            continue;
        }

        escarp_array_delete(arr);
    }
    return test_failure;
}

typedef struct test_case_array_push_t {
    escarp_array_t *arr;
    unsigned char *in;

    escarp_array_error_t expected_err;
    size_t expected_len;
} test_case_array_push_t;

size_t test_array_push(void) {
    size_t test_failure = 0;

    escarp_array_t *null_arr = NULL;
    escarp_array_t *zero_size_arr_0 = NULL;
    escarp_array_t *zero_size_arr_1 = NULL;
    escarp_array_t *one_size_arr_0 = NULL;
    unsigned char one_size_arr_[] = {1};

    unsigned char zero = 0;

    test_case_array_push_t test_cases[] = {
        {null_arr, &zero, ESCARP_ARRAY_ERROR_NULL_POINTER},
        {zero_size_arr_0, NULL, ESCARP_ARRAY_ERROR_NULL_POINTER},
        {zero_size_arr_1, &zero, ESCARP_ARRAY_SUCCESS, 1},
        {one_size_arr_0, &zero, ESCARP_ARRAY_SUCCESS, 2}};
    size_t test_case_len = sizeof(test_cases) / sizeof(test_case_array_push_t);
    size_t i = 0;

    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&zero_size_arr_0, sizeof(unsigned char), NULL, 0));
    test_cases[1].arr = zero_size_arr_0;
    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&zero_size_arr_1, sizeof(unsigned char), NULL, 0));
    test_cases[2].arr = zero_size_arr_1;
    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&one_size_arr_0, sizeof(unsigned char),
                             one_size_arr_, sizeof(one_size_arr_)));
    test_cases[3].arr = one_size_arr_0;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_array_t *arr = test_cases[i].arr;
        unsigned char *in = test_cases[i].in;
        escarp_array_error_t actual_err = ESCARP_ARRAY_SUCCESS;
        size_t actual_len = 0;
        unsigned char actual_pushed = 0;

        actual_err = escarp_array_push(arr, in);
        if (test_cases[i].expected_err != actual_err) {
            fprintf(stderr, "index: %d, expected_err: %d, actual_err: %d\n", i,
                    test_cases[i].expected_err, actual_err);
            escarp_array_delete(arr);
            continue;
        }
        if (actual_err != ESCARP_ARRAY_SUCCESS) {
            escarp_array_delete(arr);
            continue;
        }

        assert(ESCARP_ARRAY_SUCCESS ==
               escarp_array_get_length(arr, &actual_len));
        if (test_cases[i].expected_len != actual_len) {
            fprintf(stderr, "index: %d, expected_len: %d, actual_len: %d\n", i,
                    test_cases[i].expected_len, actual_len);
            escarp_array_delete(arr);
            continue;
        }

        assert(ESCARP_ARRAY_SUCCESS ==
               escarp_array_at(arr, actual_len - 1, &actual_pushed));
        if (*(test_cases[i].in) != actual_pushed) {
            fprintf(stderr, "index: %d, in: %d, actual_pushed: %d\n", i,
                    *(test_cases[i].in), actual_pushed);
            escarp_array_delete(arr);
            continue;
        }

        escarp_array_delete(arr);
    }
    return test_failure;
}

typedef struct test_case_array_pop_t {
    escarp_array_t *arr;

    escarp_array_error_t expected_err;
    unsigned char expected_out;
    size_t expected_len;
} test_case_array_pop_t;

size_t test_array_pop(void) {
    size_t test_failure = 0;

    escarp_array_t *arr_1 = NULL;
    unsigned char arr_1_[] = {};

    escarp_array_t *arr_2 = NULL;
    unsigned char arr_2_[] = {0};

    escarp_array_t *arr_3 = NULL;
    unsigned char arr_3_[] = {0, 1};

    test_case_array_pop_t test_cases[] = {
        {NULL, ESCARP_ARRAY_ERROR_NULL_POINTER},
        {arr_1, ESCARP_ARRAY_ERROR_OUT_OF_BOUNDS},
        {arr_2, ESCARP_ARRAY_SUCCESS, 0, 0},
        {arr_3, ESCARP_ARRAY_SUCCESS, 1, 1}};
    size_t test_case_len = sizeof(test_cases) / sizeof(test_case_array_pop_t);
    size_t i = 0;

    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&arr_1, sizeof(unsigned char), arr_1_, 0));
    test_cases[1].arr = arr_1;

    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&arr_2, sizeof(unsigned char), arr_2_, sizeof(arr_2_)));
    test_cases[2].arr = arr_2;

    assert(ESCARP_ARRAY_SUCCESS ==
           escarp_array_new(&arr_3, sizeof(unsigned char),
                             arr_3_, sizeof(arr_3_)));
    test_cases[3].arr = arr_3;

    printf("* %s\n", __func__);

    for (i = 0; i < test_case_len; i++) {
        escarp_array_t *arr = test_cases[i].arr;
        escarp_array_error_t actual_err = ESCARP_ARRAY_SUCCESS;
        unsigned char actual_out = 0;
        size_t actual_len = 0;

        actual_err = escarp_array_pop(arr, &actual_out);
        if (test_cases[i].expected_err != actual_err) {
            fprintf(stderr, "index: %d, expected_err: %d, actual_err: %d\n", i,
                    test_cases[i].expected_err, actual_err);
            escarp_array_delete(arr);
            continue;
        }
        if (actual_err != ESCARP_ARRAY_SUCCESS) {
            escarp_array_delete(arr);
            continue;
        }

        if (test_cases[i].expected_out != actual_out) {
            fprintf(stderr, "index: %d, expected_out: %d, actual_out: %d\n", i,
                    test_cases[i].expected_out, actual_out);
            escarp_array_delete(arr);
            continue;
        }

        assert(ESCARP_ARRAY_SUCCESS ==
               escarp_array_get_length(arr, &actual_len));
        if (test_cases[i].expected_len != actual_len) {
            fprintf(stderr, "index: %d, expected_len: %d, actual_len: %d\n", i,
                    test_cases[i].expected_len, actual_len);
            escarp_array_delete(arr);
            continue;
        }

        escarp_array_delete(arr);
    }
    return test_failure;
}

int main(void) {
    size_t test_failure = 0;

    test_failure += test_array_new();
    test_failure += test_array_at();
    test_failure += test_array_push();
    test_failure += test_array_pop();

    test_failure += test_stream();

    test_failure += test_any_new();
    test_failure += test_value_new();
    test_failure += test_sequence_new();
    test_failure += test_parse();

    printf("%d\n", test_failure);
    return 0;
}