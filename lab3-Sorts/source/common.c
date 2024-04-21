#include "../include/common.h"

int int_cmp (void* elem1,
             void* elem2)
{
    assert (elem1);
    assert (elem2);

    if (*(int*)elem1 > *(int*)elem2) return GREATER;
    if (*(int*)elem1 < *(int*)elem2) return LESS;
    return EQUAL;
}

void
int_swap (int* first, int* second)
{
    assert (first);
    assert (second);

    int temp_value = *second;
    *second = *first;
    *first  = temp_value;
}
