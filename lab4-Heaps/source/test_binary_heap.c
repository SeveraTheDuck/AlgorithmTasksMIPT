#include "kary_heap.h"
#include <limits.h>
#include <stdio.h>
#include <time.h>



const size_t KARY_HEAP_K = 2;
const int HEAP_INFINITY_VALUE = INT_MAX;



enum int_cmp_status
{
    INT_CMP_LESS    = -1,
    INT_CMP_EQUAL   =  0,
    INT_CMP_GREATER =  1
};



int CompareInt (void* a, void* b)
{
    assert (a);
    assert (b);

    int a_value = *(int*)a;
    int b_value = *(int*)b;

    if (a_value < b_value) return INT_CMP_LESS;
    if (a_value > b_value) return INT_CMP_GREATER;
    return INT_CMP_EQUAL;
}


clock_t
KHeapRegularInserts (const dynamic_array* const d_array)
{
    assert (d_array);

    const size_t elem_number = d_array->data_array_size;

    k_heap* const heap = KHeapConstructor (NULL, KARY_HEAP_K, sizeof (int),
                                           HEAP_INFINITY_VALUE, CompareInt); 
    assert (heap);

    for (size_t i = 0; i < elem_number; ++i)
        KHeapInsert (heap, DynamicArrayGetElemPtrByIndex (d_array, i));

    return heap;
}

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

    heap->d_array->save_array = DYNAMIC_ARRAY_DESTROY;
    heap = KHeapDestructor (heap);

    return end - begin;
}

int main (void)
{
    size_t elem_number = 0;
    int input = 0;
    assert (scanf ("%zd", &elem_number) == 1);

    dynamic_array* array = DynamicArrayConstructor (elem_number, sizeof (int),
                                                    DYNAMIC_ARRAY_REALLOC_ENABLED,
                                                    DYNAMIC_ARRAY_DESTROY);
    assert (array);

    for (size_t i = 0; i < elem_number; ++i)
    {
        assert (scanf ("%d", &input) == 1);
        DynamicArrayPush (array, &input);
    }

    printf ("%ld\n", KHeapLinear (array));

    return 0;
}