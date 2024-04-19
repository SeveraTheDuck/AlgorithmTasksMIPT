#include "kary_heap.h"
#include "common.h"
#include <limits.h>
#include <stdio.h>
#include <time.h>



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
    dynamic_array* array = DynamicArrayConstructor (elem_number, sizeof (int),
                                                    DYNAMIC_ARRAY_REALLOC_ENABLED,
                                                    DYNAMIC_ARRAY_DESTROY);
    assert (array);

    int input = 0;

    for (size_t i = 0; i < elem_number; ++i)
    {
        assert (scanf ("%d", &input) == 1);
        DynamicArrayPush (array, &input);
    }

    return array;
}



int main (void)
{
    size_t elem_number = 0;
    assert (scanf ("%zd", &elem_number) == 1);

    dynamic_array* const d_array = ReadDynamicArray (elem_number);
    assert (d_array);

    printf ("%ld\n", KHeapLinear (d_array));
    // array is being freed during KHeapLinear ()

    return 0;
}
