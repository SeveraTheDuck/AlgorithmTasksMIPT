#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>


#undef RAND_MAX
#define RAND_MAX INT_MAX


//-----------------------------------------------------------------------------
// Order statistic functions' prototypes
//-----------------------------------------------------------------------------
unsigned long long
OrderStatistic (unsigned long long* const array, 
                const size_t elem_number,
                const size_t k);

size_t
LomutoPartition (unsigned long long* const   array,
                 const size_t left_index,
                 const size_t right_index);

size_t
RandomPivot (const size_t left_index,
             const size_t right_index);

void
SwapIntegers (unsigned long long* const elem1,
              unsigned long long* const elem2);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Order statistic functions' definitions
//-----------------------------------------------------------------------------
unsigned long long
OrderStatistic (unsigned long long* const array,
                const size_t elem_number,
                const size_t k)
{
    assert (array);

    size_t left_index  = 0;
    size_t right_index = elem_number - 1;
    size_t pivot_index = 0;

    while (true)
    {
        pivot_index = LomutoPartition (array, left_index, right_index);

        if (left_index == right_index) 
            return array[left_index];

        if (pivot_index == k - 1)
            return array[pivot_index];

        if (k - 1 < pivot_index)
            right_index = pivot_index;

        else left_index = pivot_index + 1;
    }

    return array[pivot_index];
}

size_t
LomutoPartition (unsigned long long* const   array,
                 const size_t left_index,
                 const size_t right_index)
{
    assert (array);
    
    if (left_index >= right_index)
        return left_index;

    size_t pivot = RandomPivot (left_index, right_index);
    size_t i = left_index;

    unsigned long long pivot_elem = array[pivot];

    SwapIntegers (&array[right_index], &array[pivot]);

    for (size_t j = left_index; j <= right_index; ++j)
    {
        if (array[j] < pivot_elem)
            SwapIntegers (&array[i++], &array[j]);
    }

    SwapIntegers (&array[i], &array[right_index]);

    return i;
}

size_t
RandomPivot (const size_t left_index,
             const size_t right_index)
{
    assert (left_index <= right_index);

    return left_index + rand() / (RAND_MAX / (right_index - left_index + 1) + 1);
}

void
SwapIntegers (unsigned long long* const elem1,
              unsigned long long* const elem2)
{
    assert (elem1);
    assert (elem2);

    unsigned long long tmp_elem = *elem1;
    *elem1 = *elem2;
    *elem2 = tmp_elem;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------
const unsigned long long  FIRST_MULTIPLIER = 123;
const unsigned long long SECOND_MULTIPLIER = 45;
const unsigned long long MODULE_VALUE = 10000000 + 4321;

void
FillArray (unsigned long long* const a,
           const size_t N);

int main ()
{
    size_t n = 0;
    scanf ("%zd", &n);

    size_t k = 0;
    scanf ("%zd", &k);

    unsigned long long* a =
        (unsigned long long*) calloc (n, sizeof (unsigned long long));
    assert (a);

    scanf ("%llu", &a[0]);
    scanf ("%llu", &a[1]);

    FillArray (a, n);

    printf ("%llu\n", OrderStatistic (a, n, k));

    free (a);
    return 0;
}

void
FillArray (unsigned long long* const a,
           const size_t n)
{
    assert (a);

    for (size_t i = 2; i < n; ++i)
    {
        a[i]  = (a[i - 1] *  FIRST_MULTIPLIER) % MODULE_VALUE +
                (a[i - 2] * SECOND_MULTIPLIER) % MODULE_VALUE;
        a[i] %= MODULE_VALUE;
    }
}