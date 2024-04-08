#ifndef BINOMIAL_HEAP_H
#define BINOMIAL_HEAP_H

#include <stdlib.h>
#include <assert.h>



typedef
struct binomial_heap_node
{
    int key;
    struct binomial_heap_node* parent;
    struct binomial_heap_node* child;
    struct binomial_heap_node* sibling;
    size_t degree;
}
binomial_heap_node;

enum binomial_heap_error_status
{
    BINOMIAL_HEAP_SUCCESS = 0,
    BINOMINAL_HEAP_ERROR  = 1
};

typedef size_t binomial_heap_error_t;



binomial_heap_node*
BinomialHeapDestructor (binomial_heap_node* const heap);

binomial_heap_node*
BinomialHeapMerge (binomial_heap_node* const heap1,
                   binomial_heap_node* const heap2);

binomial_heap_node*
BinomialHeapInsert (binomial_heap_node* const heap,
                    const int key);

binomial_heap_node*
BinomialHeapNodeConstructor (const int key);

binomial_heap_node*
BinomialHeapNodeDestructor (binomial_heap_node* const node);



#endif