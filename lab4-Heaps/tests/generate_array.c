#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// process name and number of elements
const int      MIN_ARG_NUMBER        = 2;
const size_t   ELEM_NUMBER_ARG_INDEX = 1;
const unsigned SEED_VALUE            = 42;

int main (const int argc, const char** const argv)
{
    assert (argc >= MIN_ARG_NUMBER);
    assert (argv);
    assert (argv[ELEM_NUMBER_ARG_INDEX]);

    // read elem_number
    const size_t elem_number =
        (size_t) atoll (argv[ELEM_NUMBER_ARG_INDEX]);

    srand (SEED_VALUE);

    printf ("%zd\n", elem_number);

    for (size_t i = 0; i < elem_number; ++i)
        printf ("%d\n", rand());

    return 0;
}
