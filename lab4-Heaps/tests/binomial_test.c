#include "binomial_heap.h"
#include "common.h"



clock_t
BinomialHeapTest (int* const array,
                  const size_t elem_number)
{
    assert (array);

    binomial_heap_node* root = NULL;

    clock_t begin = 0;
    clock_t end   = 0;

    begin = clock ();
    for (size_t i = 0; i < elem_number; i++)
        root = BinomialHeapInsert (root, array[i]);
    end = clock ();

    root = BinomialHeapDestructor (root);

    return end - begin;
}


int main (void)
{
    size_t elem_number = 0;
    assert (scanf ("%zd", &elem_number) == 1);

    int* const array = ReadIntArray (elem_number);
    assert (array);

    printf ("%ld\n", BinomialHeapTest (array, elem_number));

    return 0;
}
