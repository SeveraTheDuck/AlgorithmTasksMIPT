#include "../include/heapsort.h"

static struct k_heap*
HeapSortInitHeap (int* const array,
                  const size_t elem_number,
                  const size_t heap_k_value);

static int int_cmp (void* elem1,
                    void* elem2);

void
HeapSort (int* const   array,
          const size_t elem_number,
          const size_t heap_k_value)
{
    assert (array);

    /* my heapsort works with my implementation of dynamic array */
    struct k_heap* const heap =
        HeapSortInitHeap (array, elem_number, heap_k_value);
    assert (heap);

    int root_value = 0;

    for (size_t i = 0; i < elem_number; ++i)
    {
        KHeapExtractRoot (heap, &root_value);
        DynamicArrayPush (heap->d_array, &root_value);
        heap->d_array->data_array_size--;
    }

    heap->d_array->data_array_size = elem_number;
    KHeapDestructor (heap);
}

static int int_cmp (void* elem1,
                    void* elem2)
{
    assert (elem1);
    assert (elem2);

    if (*(int*)elem1 >= *(int*)elem2) return GREATER;
    return LESS;
}

static struct k_heap*
HeapSortInitHeap (int* const array,
                  const size_t elem_number,
                  const size_t heap_k_value)
{
    assert (array);

    struct dynamic_array* d_array =
        DynamicArrayConstructor (DYNAMIC_ARRAY_NULL_SIZE, sizeof (int),
                                 DYNAMIC_ARRAY_REALLOC_DISABLED,
                                 DYNAMIC_ARRAY_SAVE);
    if (d_array == NULL) return NULL;

    d_array->data_array          = (void*) array;
    d_array->data_array_capacity = elem_number;
    d_array->data_array_size     = elem_number;

    int infinite_value = INT32_MAX;

    struct k_heap* const heap =
        KHeapConstructor (d_array, heap_k_value, sizeof (int),
                          &infinite_value, int_cmp);

    if (heap == NULL)
        DynamicArrayDestructor (d_array);

    return heap;
}
