#include "make_array.h"

void*
MakeArray (const size_t   n_elements,
           const unsigned max_value);

static unsigned
RandomWithMaxValue (const unsigned max_value);

const int REFERENCE_ARGC = 3;

int main (const int argc, const char** const argv)
{
    assert (argc == REFERENCE_ARGC);

    const size_t n_elements = strtol (argv [...]);
}

void*
MakeArray (const size_t   n_elements,
           const unsigned max_value)
{
    assert (n_elements > 0);
    assert (max_value  > 0);

    unsigned* const array = 
        (unsigned*) calloc (n_elements, sizeof (unsigned));
    if (array == 0) return NULL;

    for (size_t i = 0; i < n_elements; ++i)
    {
        array [i] = RandomWithMaxValue (max_value);
    }
}

static unsigned
RandomWithMaxValue (const unsigned max_value)
{
    assert (max_value > 0);
    return (unsigned) ((double) rand() / ((double) RAND_MAX + 1) * max_value);
}