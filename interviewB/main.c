#include <stdio.h>
#include <assert.h>

int main (void)
{
    size_t n = 0;
    size_t normal_xor = 0;
    size_t given_xor  = 0;

    size_t given_number = 0;

    assert (scanf ("%zu", &n) == 1);
    normal_xor = n;

    for (size_t i = 0; i < n; ++i)
    {
        normal_xor ^= i;
        assert (scanf ("%zu", &given_number) == 1);
        given_xor ^= given_number;
    }

    printf ("%zu\n", normal_xor ^ given_xor);

    return 0;
}