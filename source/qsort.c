#include "../include/qsort.h"

static void
QuickSort (int* const   array,
           const size_t left_index,
           const size_t right_index,
           size_t (*partition) (int* const   array,
                                const size_t left_index,
                                const size_t right_index));

static size_t
LomutoPartition (int* const   array,
                 const size_t left_index,
                 const size_t right_index);

static size_t
HoarePartition (int* const   array,
                const size_t left_index,
                const size_t right_index);

static void
QsortRecursionThickPartition (int* const   array,
                              const size_t left_index,
                              const size_t right_index);

static void
ThickPartition (int* const   array,
                size_t left_index,
                size_t right_index,
                size_t* const equals_from,
                size_t* const equals_to);

static size_t
MiddlePivot (int* const   array,
             const size_t left_index,
             const size_t right_index);

static void
int_swap (int* first, int* second);

void
QuickLomutoSort (int* const   array,
                 const size_t elem_number)
{
    if (array == NULL) return;

    QuickSort (array, 0, elem_number - 1, LomutoPartition);
}

void
QuickHoareSort (int* const   array,
                const size_t elem_number)
{
    if (array == NULL) return;

    QuickSort (array, 0, elem_number - 1, HoarePartition);
}

void
QuickThickSort (int* const   array,
                const size_t elem_number)
{
    if (array == NULL) return;

    QsortRecursionThickPartition (array, 0, elem_number - 1);
}

static void
QuickSort (int* const   array,
           const size_t left_index,
           const size_t right_index,
           size_t (*partition) (int* const   array,
                                const size_t left_index,
                                const size_t right_index))
{
    assert (array);

    if (left_index >= right_index) return;

    size_t pivot = partition (array, left_index, right_index);

    QuickSort (array, left_index, pivot,       partition);
    QuickSort (array, pivot + 1,  right_index, partition);
}

static size_t
LomutoPartition (int* const   array,
                 const size_t left_index,
                 const size_t right_index)
{
    assert (array);
    assert (left_index < right_index);

    size_t pivot = MiddlePivot (array, left_index, right_index);
    size_t i = left_index;

    int pivot_elem = array[pivot];

    int_swap (&array[right_index], &array[pivot]);

    for (size_t j = left_index; j < right_index; ++j)
    {
        if (array[j] <= pivot_elem)
            int_swap (&array[i++], &array[j]);
    }

    int_swap (&array[i], &array[right_index]);

    return i;
}

static size_t
HoarePartition (int* const   array,
                const size_t left_index,
                const size_t right_index)
{
    assert (array);
    assert (left_index < right_index);

    size_t pivot = MiddlePivot (array, left_index, right_index);
    int pivot_elem = array[pivot];

    size_t i = left_index;
    size_t j = right_index;

    while (i <= j)
    {
        while (array[i] < pivot_elem) i++;
        while (array[j] > pivot_elem) j--;

        if (i >= j) return j;

        int_swap (&array[i++], &array[j--]);
    }

    return j;
}

static void
QsortRecursionThickPartition (int* const   array,
                              const size_t left_index,
                              const size_t right_index)
{
    assert (array);

    if (left_index >= right_index) return;

    size_t equals_from = 0;
    size_t equals_to   = 0;

    ThickPartition (array, left_index, right_index, &equals_from, &equals_to);

    QsortRecursionThickPartition (array, left_index,    equals_from);
    QsortRecursionThickPartition (array, equals_to + 1, right_index);
}

static void
ThickPartition (int* const   array,
                size_t left_index,
                size_t right_index,
                size_t* const equals_from,
                size_t* const equals_to)
{
    assert (array);
    assert (equals_from);
    assert (equals_to);

    size_t pivot = MiddlePivot (array, left_index, right_index);
    int pivot_elem = array[pivot];

    size_t mid_index = left_index;

    while (mid_index <= right_index)
    {
        if (array[mid_index] < pivot_elem)
            int_swap (&array[left_index++], &array[mid_index++]);

        else if (array[mid_index] == pivot_elem)
            mid_index++;

        else
            int_swap (&array[mid_index], &array[right_index--]);
    }

    *equals_from = left_index;
    *equals_to   = right_index;
}

static size_t
MiddlePivot (int* const   array,
             const size_t left_index,
             const size_t right_index)
{
    assert (array);
    assert (left_index < right_index);

    return left_index + (right_index - left_index) / 2;
}

static void
int_swap (int* first, int* second)
{
    assert (first);
    assert (second);

    int temp_value = *second;
    *second = *first;
    *first  = temp_value;
}
