#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



//-----------------------------------------------------------------------------
// Enumeration with the result of a comparison
//-----------------------------------------------------------------------------
enum comparator_result
{
    LESS    = -1,
    EQUAL   = 0,
    GREATER = 1
};
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Sort functions' prototypes
//-----------------------------------------------------------------------------
void
QuickSort (void* const  array,
           const size_t elem_number,
           const size_t elem_size,
           int (*comparator)(const void* const,
                             const void* const));

void
QSortRecur (void* const  array,
            const size_t left_index,
            const size_t right_index,
            const size_t elem_size,
            void* const  pivot_container,
            int (*comparator)(const void* const,
                              const void* const));

size_t
HoarePartition (void* const  array,
                const size_t left_index,
                const size_t right_index,
                const size_t elem_size,
                void* const  pivot_container,
                int (*comparator)(const void* const,
                                  const void* const));

size_t
RandomPivotIndex (const size_t left_index,
                  const size_t right_index);

void
SwapElements (void* const  elem1,
              void* const  elem2,
              const size_t elem_size);

int
CompareInt (const void* const elem1,
            const void* const elem2);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Sort functions' definitions
//-----------------------------------------------------------------------------
void
QuickSort (void* const  array,
           const size_t elem_number,
           const size_t elem_size,
           int (*comparator)(const void* const,
                             const void* const))
{
    if (array      == NULL) return;
    if (comparator == NULL) return;

    void* const pivot_container = calloc (1, elem_size);
    if (pivot_container == NULL) return;

    QSortRecur (array, 0, elem_number - 1, elem_size,
                pivot_container, comparator);

    free (pivot_container);
}

void
QSortRecur (void* const  array,
            const size_t left_index,
            const size_t right_index,
            const size_t elem_size,
            void* const  pivot_container,
            int (*comparator)(const void* const,
                              const void* const))
{
    assert (array);
    assert (pivot_container);
    assert (comparator);

    if (left_index >= right_index) return;

    size_t pivot_index = HoarePartition (array, left_index, right_index, elem_size,
                                         pivot_container, comparator);

    QSortRecur (array, left_index, pivot_index, elem_size,
                pivot_container, comparator);

    QSortRecur (array, pivot_index + 1, right_index, elem_size,
                pivot_container, comparator);
}

size_t
HoarePartition (void* const  array,
                const size_t left_index,
                const size_t right_index,
                const size_t elem_size,
                void* const  pivot_container,
                int (*comparator)(const void* const,
                                  const void* const))
{
    assert (array);
    assert (pivot_container);
    assert (comparator);
    assert (left_index < right_index);

    size_t pivot_index = RandomPivotIndex (left_index, right_index);
    void* const pivot_elem = (void*)((char*)array + pivot_index * elem_size);

    memcpy (pivot_container, pivot_elem, elem_size);

    size_t i = left_index;
    size_t j = right_index;
    char*  i_ptr = (char*)array + i * elem_size;
    char*  j_ptr = (char*)array + j * elem_size;

    while (i <= j)
    {
        while (comparator ((void*)i_ptr, pivot_container) < 0)
        {
            i_ptr += elem_size;
            i++;
        }

        while (comparator ((void*)j_ptr, pivot_container) > 0)
        {
            j_ptr -= elem_size;
            j--;
        }

        if (i >= j) return j;

        SwapElements (i_ptr, j_ptr, elem_size);
        i_ptr += elem_size;
        j_ptr -= elem_size;
        i++;
        j--;
    }

    return j;
}

size_t
RandomPivotIndex (const size_t left_index,
                  const size_t right_index)
{
    assert (left_index < right_index);

    size_t pivot_index = left_index +
        (size_t)rand() / (RAND_MAX / (right_index - left_index + 1) + 1);

    return pivot_index;
}

void
SwapElements (void* const  elem1,
              void* const  elem2,
              const size_t elem_size)
{
    assert (elem1);
    assert (elem2);

    void* temp = calloc (1, elem_size);
    if (temp == NULL) return;

    memcpy (temp,  elem1, elem_size);
    memcpy (elem1, elem2, elem_size);
    memcpy (elem2, temp,  elem_size);

    free (temp);
}

int
CompareInt (const void* const elem1,
            const void* const elem2)
{
    assert (elem1);
    assert (elem2);

    const int* const int1 = (const int* const)elem1;
    const int* const int2 = (const int* const)elem2;

    if (*int1 < *int2) return LESS;
    if (*int1 > *int2) return GREATER;

    return EQUAL;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main function
//-----------------------------------------------------------------------------
int main (void)
{
    size_t N = 0;
    scanf ("%zd", &N);

    int* a = (int*) calloc (N, sizeof (int));
    assert (a);

    for (size_t i = 0; i < N; ++i)
        scanf ("%d", &a[i]);

    QuickSort (a, N, sizeof (int), CompareInt);

    for (size_t i = 0; i < N; ++i)
        printf ("%d ", a[i]);

    free (a);
    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
