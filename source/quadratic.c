#include "../include/quadratic.h"
#include <stdio.h>

const size_t SHELL_SORT_KNUT_INIT_STEP       = 1;
const size_t SHELL_SORT_KNUT_STEP_MULTIPLIER = 3;
const size_t SHELL_SORT_KNUT_ADDITIVE        = 1;

void
BubbleSort (int* const   array,
            const size_t elem_number)
{
    if (array == NULL) return;

    size_t num_of_swaps = 0;

    for (size_t i = 0; i < elem_number; ++i)
    {
        num_of_swaps = 0;

        for (size_t j = 1; j < elem_number - i; ++j)
        {
            if (array[j - 1] > array[j])
            {
                int_swap (&array[j - 1], &array[j]);
                ++num_of_swaps;
            }
        }

        if (num_of_swaps == 0) return;
    }
}

void
InsertionSort (int* const   array,
               const size_t elem_number)
{
    if (array == NULL) return;

    for (size_t i = 1; i < elem_number; ++i)
    {
        /* reverse loop does j-- before loop body, so j = i, not i - 1 */
        for (size_t j = i; j-- > 0;)
        {
            if (array[j] > array[j + 1])
                int_swap (&array[j], &array[j + 1]);

            else break;
        }
    }
}

void
SelectionSort (int* const   array,
               const size_t elem_number)
{
    if (array == NULL) return;

    size_t min_index = 0;

    for (size_t i = 0; i < elem_number - 1; ++i)
    {
        min_index = i;

        for (size_t j = i + 1; j < elem_number; ++j)
        {
            if (array[j] < array[min_index])
                min_index = j;
        }

        if (min_index != i)
            int_swap (&array[i], &array[min_index]);
    }
}

void
ShellSort (int* const   array,
           const size_t elem_number)
{
    if (array == NULL) return;

    size_t step = SHELL_SORT_KNUT_INIT_STEP;
    size_t step_max = elem_number / (SHELL_SORT_KNUT_STEP_MULTIPLIER *
                                     SHELL_SORT_KNUT_STEP_MULTIPLIER);

    while (step <= step_max)
    {
        step = step * SHELL_SORT_KNUT_STEP_MULTIPLIER + SHELL_SORT_KNUT_ADDITIVE;
    }

    int temp_value = 0;

    for (; step > 0; step /= SHELL_SORT_KNUT_STEP_MULTIPLIER)
    {
        for (size_t i = step; i < elem_number; i += step)
        {
            size_t j = i;
            temp_value = array[i];

            for (; j >= step && array[j - step] > temp_value; j -= step)
            {
                array[j] = array[j - step];
            }

            array[j] = temp_value;
        }
    }
}
