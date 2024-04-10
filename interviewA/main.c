#include <stdio.h>
#include <stdint.h>
#include <assert.h>



uint32_t
CountBits (uint32_t x);



int main (void)
{
    uint32_t x = 0;
    uint32_t y = 0;

    assert (scanf ("%u %u", &x, &y) == 2);

    printf ("%u\n", CountBits (x ^ y));

    return 0;
}

uint32_t
CountBits (uint32_t x)
{
    x = (x & 0x55555555u) + ((x >> 1) & 0x55555555u);
    x = (x & 0x33333333u) + ((x >> 2) & 0x33333333u);
    x = (x & 0x0f0f0f0fu) + ((x >> 4) & 0x0f0f0f0fu);
    return x % 255u;
}