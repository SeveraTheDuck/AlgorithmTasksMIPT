#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum int_cmp_status
{
    LESS    = -1,
    EQUAL   =  0,
    GREATER =  1
};

int CompareInt (const void* elem1,
                const void* elem2)
{
    assert (elem1);
    assert (elem2);

    if (*(int*)elem1 > *(int*) elem2) return GREATER;
    if (*(int*)elem1 < *(int*) elem2) return LESS;
    return EQUAL;
}

int main ()
{
    size_t elem_number = 0;
    scanf ("%zd", &elem_number);

    int* const array =
        (int* const) calloc (elem_number, sizeof (int));
    assert (array);

    for (size_t i = 0; i < elem_number; ++i)
        scanf ("%d", &array[i]);

    qsort (array, elem_number, sizeof (int), CompareInt);

    for (size_t i = 0; i < elem_number; ++i)
        printf ("%d\n", array[i]);

    free (array);
    return 0;
}
