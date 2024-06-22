#include <escarp.h>

#include <stdio.h>

#include "stream.h"
#include "parser.h"

int main(void) {
    size_t test_failure = 0;

    test_failure += test_stream();

    test_failure += test_any_new();
    test_failure += test_value_new();
    test_failure += test_sequence_new();
    test_failure += test_parse();

    printf("%d\n", test_failure);
    return 0;
}