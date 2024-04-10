#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



int*
ReadArray (const size_t n);

void
FindTwoNotRepeating (const int* const numbers,
                     const size_t n,
                     int* const first,
                     int* const second);



int main (void)
{
    size_t n = 0;
    size_t elem_num = 0;

    assert (scanf ("%zu", &n) == 1);
    elem_num = 2 * n - 2;

    int* numbers = ReadArray (elem_num);

    int first  = 0;
    int second = 0;

    FindTwoNotRepeating (numbers, elem_num, &first, &second);

    if (first <= second)
         printf ("%d %d\n", first, second);
    else printf ("%d %d\n", second, first);
    
    free (numbers);
    numbers = NULL;
    return 0;
}

int*
ReadArray (const size_t n)
{
    assert (n != 0);

    int* const numbers = (int*) malloc (n * sizeof (int));
    assert (numbers);

    for (size_t i = 0; i < n; ++i)
        assert (scanf ("%d", &numbers[i]) == 1);

    return numbers;
}

void
FindTwoNotRepeating (const int* const numbers,
                     const size_t n,
                     int* const first,
                     int* const second)
{
    assert (numbers);
    assert (first);
    assert (second);

    int given_xor = 0;
    int check_bit = 0;

    for (size_t i = 0; i < n; ++i)
        given_xor ^= numbers[i];

    check_bit = given_xor & ~(given_xor - 1); // first 1 bit from right

    for (size_t i = 0; i < n; ++i)
    {
        if ((numbers[i] & check_bit) == 0)
            *first  ^= numbers[i];
        else
            *second ^= numbers[i];
    }
}