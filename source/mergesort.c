#include "../include/mergesort.h"

static void
MergeSortRecursion (int* const   array,
                    int* const   temp_array,
                    const size_t left_index,
                    const size_t right_index);

static void
MergeParts (int* const   array,
            int* const   temp_array,
            const size_t left_index,
            const size_t mid_index,
            const size_t right_index);

static size_t
SizeMin (const size_t elem1,
         const size_t elem2);

void
MergeRecursiveSort (int* const   array,
                    const size_t elem_number)
{
    if (array == NULL) return;

    int* const temp_array = (int*) calloc (elem_number, sizeof (int));
    if (temp_array == NULL) return;

    MergeSortRecursion (array, temp_array, 0, elem_number);

    free (temp_array);
}

void
MergeIterativeSort (int* const   array,
                    const size_t elem_number)
{
    if (array == NULL) return;

    int* const temp_array = (int*) calloc (elem_number, sizeof (int));
    if (temp_array == NULL) return;


    for (size_t i = 1; i < elem_number; i *= 2)
        for (size_t j = 0; j < elem_number - i; j += 2 * i)
            MergeParts (array, temp_array, j, j + i,
                        SizeMin (j + 2 * i, elem_number));

    free (temp_array);
}

static void
MergeSortRecursion (int* const   array,
                    int* const   temp_array,
                    const size_t left_index,
                    const size_t right_index)
{
    assert (array);
    assert (temp_array);
    assert (left_index <= right_index);

    if (right_index - left_index <= 1) return;

    size_t mid_index = (left_index + right_index) / 2;

    MergeSortRecursion (array, temp_array, left_index, mid_index);
    MergeSortRecursion (array, temp_array, mid_index,  right_index);
    MergeParts (array, temp_array, left_index, mid_index, right_index);
}

static void
MergeParts (int* const   array,
            int* const   temp_array,
            const size_t left_index,
            const size_t mid_index,
            const size_t right_index)
{
    assert (array);
    assert (temp_array);
    assert (left_index <= right_index);

    size_t first_part_i  = left_index;   // [left, mid)  iterator
    size_t second_part_i = mid_index;    // [mid, right) iterator
    bool copy_left_part = false;

    for (size_t i = left_index; i < right_index; ++i)
    {
        copy_left_part = first_part_i  <  mid_index   &&
                        (second_part_i >= right_index ||
                         array[first_part_i] < array[second_part_i]);

        if (copy_left_part)
            temp_array[i] = array[first_part_i++];
        
        else
            temp_array[i] = array[second_part_i++];
    }

    for (size_t i = left_index; i < right_index; ++i)
        array[i] = temp_array[i];
}

static size_t
SizeMin (const size_t elem1,
         const size_t elem2)
{
    if (elem1 <= elem2) return elem1;
    else return elem2; 
}