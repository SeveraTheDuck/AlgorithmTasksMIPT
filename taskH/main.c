#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



//-----------------------------------------------------------------------------
// Sort functions' prototypes
//-----------------------------------------------------------------------------
void
QuickSort (int* const array, const size_t elem_number);

void
QSortRecur (int* const   array,
            const size_t left_index,
            const size_t right_index);

size_t
HoarePartition (int* const   array,
                const size_t left_index,
                const size_t right_index);

size_t
RandomPivot (const size_t left_index,
             const size_t right_index);

void
SwapIntegers (int* const elem1,
              int* const elem2);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Sort functions' definitions
//-----------------------------------------------------------------------------
void
QuickSort (int* const array, const size_t elem_number)
{
    if (array == NULL) return;

    QSortRecur (array, 0, elem_number - 1);
}

void
QSortRecur (int* const   array,
            const size_t left_index,
            const size_t right_index)
{
    assert (array);

    if (left_index >= right_index) return;

    size_t pivot = HoarePartition (array, left_index, right_index);

    QSortRecur (array, left_index, pivot);
    QSortRecur (array, pivot + 1,  right_index);
}

size_t
HoarePartition (int* const   array,
                const size_t left_index,
                const size_t right_index)
{
    assert (array);
    assert (left_index < right_index);

    size_t pivot = RandomPivot (left_index, right_index);
    int pivot_elem = array[pivot];

    size_t i = left_index;
    size_t j = right_index;

    while (i <= j)
    {
        while (array[i] < pivot_elem) i++;
        while (array[j] > pivot_elem) j--;

        if (i >= j) return j;

        SwapIntegers (&array[i++], &array[j--]);
    }

    return j;
}

size_t
RandomPivot (const size_t left_index,
             const size_t right_index)
{
    assert (left_index < right_index);

    return left_index + rand() / (RAND_MAX / (right_index - left_index + 1) + 1);
}

void
SwapIntegers (int* const elem1,
              int* const elem2)
{
    assert (elem1);
    assert (elem2);

    int tmp_elem = *elem1;
    *elem1 = *elem2;
    *elem2 = tmp_elem;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------
int main ()
{
    size_t N = 0;
    scanf ("%zd", &N);

    int* a = (int*) calloc (N, sizeof (int));
    assert (a);

    for (size_t i = 0; i < N; ++i)
        scanf ("%d", &a[i]);

    QuickSort (a, N);

    for (size_t i = 0; i < N; ++i)
        printf ("%d ", a[i]);

    return 0;
}