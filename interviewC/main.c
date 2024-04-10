#include <stdio.h>
#include <assert.h>

int main (void)
{
    size_t n = 0;
    size_t elem_num = 0;
    size_t given_xor  = 0;

    size_t given_number = 0;

    assert (scanf ("%zu", &n) == 1);
    elem_num = 2 * n - 1;

    for (size_t i = 0; i < elem_num; ++i)
    {
        assert (scanf ("%zu", &given_number) == 1);
        given_xor ^= given_number;
    }

    printf ("%zu\n", given_xor);

    return 0;
}