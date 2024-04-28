#include "../include/kary_heap.h"
#include "common.h"
#include <limits.h>



const size_t KARY_HEAP_K = 2;
const int HEAP_INFINITY_VALUE = INT_MAX;



dynamic_array*
MakeDynamicFromRegularArray (int* const array,
                             const size_t elem_number)
{
    assert (array);

    dynamic_array* const d_array =
        DynamicArrayConstructor (0, sizeof (int),
                                 DYNAMIC_ARRAY_REALLOC_DISABLED,
                                 DYNAMIC_ARRAY_DESTROY);
    assert (d_array);

    d_array->array    = array;
    d_array->capacity = elem_number;

    return d_array;
}

clock_t
KHeapRegularInserts(int* const array,
                    const size_t elem_number)
{
    assert (array);

    dynamic_array* const d_array =
        MakeDynamicFromRegularArray (array, elem_number);
    assert (d_array);

    k_heap* heap = KHeapConstructor (d_array, KARY_HEAP_K, sizeof (int),
                                     &HEAP_INFINITY_VALUE, CompareInt);
    assert (heap);

    clock_t begin = 0;
    clock_t end   = 0;

    begin = clock ();
    for (size_t i = 0; i < elem_number; ++i)
        KHeapInsert (heap, &array[i]);
    end = clock();

    heap = KHeapDestructor (heap);

    return end - begin;
}

int main (void)
{
    size_t elem_number = 0;
    assert (scanf ("%zd", &elem_number) == 1);

    int* const array = ReadIntArray (elem_number);
    assert (array);

    printf ("%ld\n", KHeapRegularInserts (array, elem_number));

    return 0;
}
