#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int
MinInt (const int a, const int b);

int main ()
{
    size_t N = 0;
    scanf ("%d", &N);

    int* input_array = (int*) calloc (N, sizeof (int));
    assert (input_array);

    int* min_from_begin = (int*) calloc (N, sizeof (int));
    assert (min_from_begin);

    int* min_from_end = (int*) calloc (N, sizeof (int));
    assert (min_from_end);

    for (size_t i = 0; i < N; ++i)
        scanf ("%d", &input_array[i]);

    min_from_begin  [0] = input_array    [0];
    min_from_end[N - 1] = input_array[N - 1];

    for (size_t i = 1; i < N; ++i)
    {
        min_from_begin      [i] = MinInt (min_from_begin[i - 1], input_array        [i]);
        min_from_end[N - 1 - i] = MinInt (min_from_end  [N - i], input_array[N - 1 - i]);
    }

    size_t Q = 0;
    size_t l = 0;
    size_t r = 0;
    scanf ("%d", &Q);
    
    for (size_t i = 0; i < Q; ++i)
    {
        scanf ("%zd %zd", &l, &r);
        printf ("%d\n", MinInt (min_from_begin[l - 1], min_from_end[r - 1]));
    }

    return 0;
}

int
MinInt (const int a, const int b)
{
    if (a < b) return a;
    return b;
}