#include "../include/qsort_optimization.h"

static void
QsortLessRecursion (int* const array,
                    size_t left_index,
                    size_t right_index,
                    size_t (*find_pivot) (int* const   array,
                                          const size_t left_index,
                                          const size_t right_index));

static size_t
HoarePartitionCustomPivot (int* const   array,
                           const size_t left_index,
                           const size_t right_index,
                           size_t (*find_pivot) (int* const   array,
                                                 const size_t left_index,
                                                 const size_t right_index));

static size_t
MiddlePivot (int* const   array,
             const size_t left_index,
             const size_t right_index);

static size_t
Median3Pivot (int* const   array,
              const size_t left_index,
              const size_t right_index);

static size_t
MedianRandom3Pivot (int* const   array,
                    const size_t left_index,
                    const size_t right_index);

static int
ThreeElementsMedian (int elem1, int elem2, int elem3);

static void
int_swap (int* first, int* second);

void
QuickOneRecursionBranchSort (int* const   array,
                             const size_t elem_number)
{
    if (array == NULL) return;

    QsortLessRecursion (array, 0, elem_number - 1, MiddlePivot);
}

static void
QsortLessRecursion (int* const array,
                    size_t left_index,
                    size_t right_index,
                    size_t (*find_pivot) (int* const   array,
                                          const size_t left_index,
                                          const size_t right_index))
{
    assert (array);

    size_t pivot_index = 0;
    size_t mid_index   = 0;

    while (left_index < right_index)
    {
        pivot_index = HoarePartitionCustomPivot (array, left_index,
                                                 right_index, find_pivot);

        mid_index = MiddlePivot (array, left_index, right_index);

        if (pivot_index > mid_index)
        {
            QsortLessRecursion (array, pivot_index + 1, right_index, find_pivot);
            right_index = pivot_index;
        }

        else
        {
            QsortLessRecursion (array, left_index, pivot_index, find_pivot);
            left_index = pivot_index + 1;
        }
    }
}

static size_t
HoarePartitionCustomPivot (int* const   array,
                           const size_t left_index,
                           const size_t right_index,
                           size_t (*find_pivot) (int* const   array,
                                                 const size_t left_index,
                                                 const size_t right_index))
{
    assert (array);
    assert (left_index < right_index);

    size_t pivot = find_pivot (array, left_index, right_index);
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

static size_t
MiddlePivot (int* const   array,
             const size_t left_index,
             const size_t right_index)
{
    assert (array);
    assert (left_index < right_index);

    return left_index + (right_index - left_index) / 2;
}

static size_t
Median3Pivot (int* const   array,
              const size_t left_index,
              const size_t right_index)
{
    assert (array);
    assert (left_index < right_index);

    size_t mid_index = MiddlePivot (array, left_index, right_index);

    int median = ThreeElementsMedian (array[left_index],
                                      array[mid_index],
                                      array[right_index]);
    
    if (median == array[left_index])  return left_index;
    if (median == array[right_index]) return right_index;
    return mid_index;
}

static size_t
RandomPivot (int* const   array,
             const size_t left_index,
             const size_t right_index)
{
    assert (array);
    assert (left_index < right_index);

    return left_index + rand() / (RAND_MAX / (right_index - left_index + 1) + 1);
}

static size_t
MedianRandom3Pivot (int* const   array,
                    const size_t left_index,
                    const size_t right_index)
{
    assert (array);
    assert (left_index < right_index);

    size_t index_1 = RandomPivot (array, left_index, right_index);
    size_t index_2 = RandomPivot (array, left_index, right_index);
    size_t index_3 = RandomPivot (array, left_index, right_index);

    return ThreeElementsMedian (array[index_1], array[index_2], array[index_3]);
}

static int
ThreeElementsMedian (int elem1, int elem2, int elem3)
{
    if (elem1 > elem2) int_swap (&elem1, &elem2);
    if (elem2 > elem3) int_swap (&elem2, &elem3);

    return elem2;
}

static void
int_swap (int* first, int* second)
{
    assert (first);
    assert (second);

    *first  = *first ^ *second;
    *second = *first ^ *second;
    *first  = *first ^ *second;
}
