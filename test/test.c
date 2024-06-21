#include <escarp.h>

#include <stdio.h>

#include "any.h"
#include "value.h"
#include "sequence.h"

int main(void) {
    size_t test_failure = 0;

    test_failure += test_any_new();
    test_failure += test_any_parse();
    test_failure += test_value_new();
    test_failure += test_value_parse();
    test_failure += test_sequence_new();
    test_failure += test_sequence_parse();

    printf("%d\n", test_failure);
    return 0;
}