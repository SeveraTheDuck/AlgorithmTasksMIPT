#include "../include/bubblesort.h"

static void int_swap (int* first, int* second)
{
    assert (first);
    assert (second);

    int temp_second = *second;
    *second = *first;
    *first = temp_second;
}

void
BubbleSort (int* const   array,
            const size_t elem_number)
{
    if (array == NULL) return;

    size_t num_of_swaps = 0;

    for (size_t i = 0; i < elem_number; ++i)
    {
        num_of_swaps = 0;

        for (size_t j = 0; j < elem_number - i; ++j)
        {
            if (array [j - 1] > array [j])
            {
                int_swap (&array [j - 1], &array [j]);
                ++num_of_swaps;
            }
        }

        if (num_of_swaps == 0) return;
    }
}
