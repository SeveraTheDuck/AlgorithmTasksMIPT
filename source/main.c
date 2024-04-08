#include "binomial_heap.h"
#include <stdio.h>

int main (void)
{
    binomial_heap_node* head = NULL;

    for (size_t i = 0; i < 128; ++i)
    {
        head = BinomialHeapInsert (head, (int)i);
    }

    binomial_heap_node* cur = head->child->child;

    while (cur != NULL)
    {
        fprintf (stderr, "cur = %d\n", cur->key);
        cur = cur->sibling;
    }

    head = BinomialHeapDestructor (head);
    return 0;
}