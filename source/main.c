#include "binomial_heap.h"
#include "fib_heap.h"
#include <stdio.h>

// int main (void)
// {
//     binomial_heap_node* head = NULL;

//     for (size_t i = 0; i < 128; ++i)
//     {
//         head = BinomialHeapInsert (head, (int)i);
//     }

//     binomial_heap_node* cur = head->child->child;

//     while (cur != NULL)
//     {
//         fprintf (stderr, "cur = %d\n", cur->key);
//         cur = cur->sibling;
//     }

//     head = BinomialHeapDestructor (head);
//     return 0;
// }

const size_t N = 5;

int main (void)
{
    fib_heap* heap = FibHeapConstructor ();
    assert (heap);

    for (size_t i = 0; i < N; ++i)
    {
        FibHeapInsert (heap, 128 - (int)i);
        fprintf (stderr, "min = %d\n", FibHeapGetMin (heap));
    }
    fprintf (stderr, "\n");

    FibHeapExtractMin (heap);
    // FibHeapExtractMin (heap);

    fib_heap_node* cur = heap->min_elem;

    for (size_t i = 0; i < N - 1; ++i)
    {
        fprintf (stderr, "cur = %d\n", cur->key);
        cur = cur->right;
    }

    // for (size_t i = 0; i < 5; ++i)
    //     FibHeapExtractMin (heap);

    heap = FibHeapDestructor (heap);
    return 0;
}