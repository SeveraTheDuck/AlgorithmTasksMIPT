#include "../include/k_heap.h"
#include "common.h"

#include <limits.h>


const size_t K_HEAP_K_VALUE = 2;
const int HEAP_INFINITY_VALUE = INT_MAX;



int
CompareKHeapKeys (const k_heap_key* const a,
                  const k_heap_key* const b)
{
    assert (a);
    assert (b);

    int cmp_status = CompareInt (a->key, b->key);

    if (cmp_status == INT_CMP_LESS)    return K_HEAP_CMP_LESS;
    if (cmp_status == INT_CMP_GREATER) return K_HEAP_CMP_GREATER;
    return K_HEAP_CMP_EQUAL;
}

clock_t
KHeapLinear (d_array_t* const d_array)
{
    assert (d_array);

    clock_t begin = 0;
    clock_t end   = 0;

    k_heap_key* inf_key =
        KHeapKeyConstructor (&HEAP_INFINITY_VALUE, sizeof (int));
    assert (inf_key);

    k_heap_t* heap =
        KHeapConstructor (K_HEAP_K_VALUE, inf_key, CompareKHeapKeys);
    assert (heap);

    heap->array = DynamicArrayDestructor (heap->array);
    heap->array = d_array;
    const size_t begin_index = heap->array->size / heap->k;

    begin = clock ();
    
    for (size_t i = begin_index + 1; i-- > 0;)
        KHeapSiftDown (heap, i);

    end = clock ();

    assert (heap);

    // d_array is freed here
    inf_key = KHeapKeyDestructor (inf_key);
    heap    = KHeapDestructor    (heap);

    return end - begin;
}



d_array_t*
ReadDynamicArray (const size_t elem_number)
{
    d_array_t* const d_array = DynamicArrayConstructor (sizeof (k_heap_key*));
    assert (d_array);

    int input = 0;
    k_heap_key* key = NULL;

    for (size_t i = 0; i < elem_number; ++i)
    {
        if (scanf ("%d", &input) != 1)
        {
            for (size_t j = 0; j < d_array->size; ++j)
                KHeapKeyDestructor (DynamicArrayGetElemPtrByIndex (d_array, j));

            return DynamicArrayDestructor (d_array);
        }

        key = KHeapKeyConstructor (&input, sizeof (int));
        DynamicArrayPush (d_array, &key);
    }
    return d_array;
}



int main (void)
{
    size_t elem_number = 0;
    if (scanf ("%zd", &elem_number) != 1) return 1;

    d_array_t* const d_array = ReadDynamicArray (elem_number);
    assert (d_array);

    printf ("%ld\n", KHeapLinear (d_array));
    // d_array is freed during KHeapLinear ()

    return 0;
}
