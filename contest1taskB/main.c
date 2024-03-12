#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <limits.h>
#include <stdbool.h>

const ptrdiff_t PTRDIFF_MAX = LLONG_MAX;

bool BinarySearch (const int* begin,
                   const int* end,
                   const int  target);

int main ()
{
    // fill array
    size_t N = 0;
    scanf ("%zd", &N);

    int* const array = (int* const) calloc (N, sizeof (int));
    assert (array);

    int new_value = 0;
    for (size_t i = 0; i < N; ++i)
    {
        scanf ("%d", &new_value);
        array [i] = new_value;
    }

    // questions
    size_t Q = 0;
    scanf ("%zd", &Q);

    int  begin  = 0;
    int  end    = 0;
    int  target = 0;
    bool result = false;

    for (size_t i = 0; i < Q; ++i)
    {
        scanf ("%d %d %d", &begin, &end, &target);
        result = BinarySearch (&array [begin], &array [end], target);

        if (result == true) printf ("YES\n");
        else printf ("NO\n");
    }

    return 0;
}

bool BinarySearch (const int* begin,
                   const int* end,
                   const int  target)
{
    assert (begin);
    assert (end);
    assert (begin < end);

    ptrdiff_t difference = PTRDIFF_MAX;
    const int* cur_elem = NULL;

    while (difference > 1)
    {
        difference = end - begin;
        cur_elem = begin + difference / 2;

        if      (*cur_elem < target) begin = cur_elem;
        else if (*cur_elem > target) end   = cur_elem;
        else return true;
    }

    return false;
}