#ifndef FIB_HEAP_H
#define FIB_HEAP_H

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>


typedef
struct fib_heap_node
{
    int key;
    struct fib_heap_node* parent;
    struct fib_heap_node* child;
    struct fib_heap_node* left;
    struct fib_heap_node* right;
    size_t degree;
    bool   mark;
}
fib_heap_node;

typedef
struct fib_heap
{
    size_t size;
    fib_heap_node* min_elem;
}
fib_heap;

enum fib_heap_errors
{
    FIB_HEAP_SUCCESS = 0,
    FIB_HEAP_ERROR   = 1
};

typedef size_t fib_heap_error_t;

void
FibHeapConsolidate (fib_heap* const heap);

fib_heap*
FibHeapConstructor ();

fib_heap*
FibHeapDestructor (fib_heap* const heap);

fib_heap_node*
FibHeapNodeConstructor (const int key);

fib_heap_node*
FibHeapNodeDestructor (fib_heap_node* const node);

int
FibHeapGetMin (const fib_heap* const heap);

fib_heap_error_t
FibHeapExtractMin (fib_heap* const heap);

fib_heap_node*
FibHeapInsert (fib_heap* const heap,
               const int key);

fib_heap_error_t
FibHeapDecreaseKey (fib_heap* const heap,
                    fib_heap_node* const node,
                    const int key);

fib_heap_error_t
FibHeapDeleteKey (fib_heap* const heap,
                  fib_heap_node* const node);



#endif /* FIB_HEAP_H */