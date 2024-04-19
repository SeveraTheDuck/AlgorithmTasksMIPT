#include <stdio.h>
#include <time.h>

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

int*
ReadIntArray (const size_t elem_number)
{
    int* const array =
        (int*) malloc (elem_number * sizeof (int));
    assert (array);

    for (size_t i = 0; i < elem_number; i++)
        assert (scanf ("%d", &array[i]) == 1);

    return array;
}
