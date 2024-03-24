#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>



//-----------------------------------------------------------------------------
// Merge sort functions' prototypes
//-----------------------------------------------------------------------------
size_t
MergeIterativeSort (int* const   array,
                    const size_t elem_number);

size_t
MergeParts (int* const   array,
            int* const   temp_array,
            const size_t left_index,
            const size_t mid_index,
            const size_t right_index);

size_t
SizeMin (const size_t elem1,
         const size_t elem2);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Merge sort functions' implementations
//-----------------------------------------------------------------------------
size_t
MergeIterativeSort (int* const   array,
                    const size_t elem_number)
{
    if (array == NULL) return 0;

    int* const temp_array = (int*) calloc (elem_number, sizeof (int));
    if (temp_array == NULL) return 0;

    size_t num_of_invertions = 0;

    for (size_t i = 1; i < elem_number; i *= 2)
        for (size_t j = 0; j < elem_number - i; j += 2 * i)
            num_of_invertions += MergeParts (array, temp_array, j, j + i,
                                             SizeMin (j + 2 * i, elem_number));

    free (temp_array);
    return num_of_invertions;
}

// There is an addition in this function two calculate number of inversions
// The funcitons returns number of invertions during merge
size_t
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

    size_t num_of_inversions = 0;

    for (size_t i = left_index; i < right_index; ++i)
    {
        copy_left_part = first_part_i  <  mid_index   &&
                        (second_part_i >= right_index ||
                         array[first_part_i] < array[second_part_i]);

        if (copy_left_part)
            temp_array[i] = array[first_part_i++];
        
        else
        {
            num_of_inversions += mid_index - first_part_i;
            temp_array[i] = array[second_part_i++];
        }
    }

    for (size_t i = left_index; i < right_index; ++i)
        array[i] = temp_array[i];

    return num_of_inversions;
}

size_t
SizeMin (const size_t elem1,
         const size_t elem2)
{
    if (elem1 <= elem2) return elem1;
    else return elem2; 
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Main functions
// We use modificated merge sort for the task
//-----------------------------------------------------------------------------
int main ()
{
    size_t n = 0;
    scanf ("%zd", &n);

    unsigned* const a =
        (unsigned* const) calloc (n, sizeof (unsigned));
    assert (a);

    for (size_t i = 0; i < n; ++i)
        scanf ("%u", &a[i]);

    size_t num_of_inversions = MergeIterativeSort (a, n);

    printf ("%zd\n", num_of_inversions);

    free (a);
    return 0;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
