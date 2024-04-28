#include "../include/kary_heap.h"
#include "common.h"
#include <limits.h>



const size_t KARY_HEAP_K = 2;
const int HEAP_INFINITY_VALUE = INT_MAX;



clock_t
KHeapLinear (dynamic_array* const d_array)
{
    assert (d_array);

    clock_t begin = 0;
    clock_t end   = 0;

    begin = clock ();
    k_heap* heap = KHeapConstructor (d_array, KARY_HEAP_K, sizeof (int),
                                     &HEAP_INFINITY_VALUE, CompareInt);
    end = clock ();

    assert (heap);

    // d_array is freed here
    heap = KHeapDestructor (heap);

    return end - begin;
}



dynamic_array*
ReadDynamicArray (const size_t elem_number)
{
    int* const array = ReadIntArray (elem_number);
    assert (array);

    dynamic_array* d_array =
        DynamicArrayConstructor (0, sizeof (int),
                                 DYNAMIC_ARRAY_REALLOC_DISABLED,
                                 DYNAMIC_ARRAY_DESTROY);
    assert (d_array);

    d_array->array      = array;
    d_array->array_size = elem_number;
    d_array->capacity   = elem_number;

    return array;
}



int main (void)
{
    size_t elem_number = 0;
    assert (scanf ("%zd", &elem_number) == 1);

    dynamic_array* const d_array = ReadDynamicArray (elem_number);
    assert (d_array);

    printf ("%ld\n", KHeapLinear (d_array));
    // d_array is freed during KHeapLinear ()

    return 0;
}
