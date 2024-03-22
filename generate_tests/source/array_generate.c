#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// process name, number of elements and max possible value
const size_t MIN_ARG_NUMBER           = 3;

const size_t ELEM_NUMBER_ARG_INDEX    = 1;
const size_t MAX_POSSIBLE_VALUE_INDEX = 2;

int main (const int argc, const char** const argv)
{
    assert (argc >= MIN_ARG_NUMBER);
    assert (argv);
    assert (argv[ELEM_NUMBER_ARG_INDEX]);
    assert (argv[MAX_POSSIBLE_VALUE_INDEX]);

    // read elem_number
    const size_t elem_number =
        (size_t) atoll (argv[ELEM_NUMBER_ARG_INDEX]);
    assert (elem_number > 0);

    const int max_value = atoi (argv[MAX_POSSIBLE_VALUE_INDEX]);

    // time (NULL) doesn't change fast enough
    // adding pointer value is kinda random enough
    srand ((size_t)time (NULL) + (size_t) argv);

    printf ("%zd\n", elem_number);

    for (size_t i = 0; i < elem_number; ++i)
        printf ("%d\n", rand() / (RAND_MAX / (max_value + 1) + 1));

    return 0;
}
