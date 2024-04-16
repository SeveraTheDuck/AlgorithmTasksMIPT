#include "../include/fib_heap.h"
#include "stdio.h" // DEBUGGGGGGGGGGGGGGGGGGGg
const int FIB_HEAP_POISON = INT_MAX;



static void
MakeNullParentsForChildren (fib_heap_node* const node);

static fib_heap_node*
UnionCyclicLists (fib_heap_node* const first,
                  fib_heap_node* const second);



static void
FibHeapCut (fib_heap* const heap,
            fib_heap_node* const node);

static void
FibHeapCascadingCut (fib_heap* const heap,
                     fib_heap_node*  node);

static void
FibHeapSwapNodes (fib_heap_node* const node1,
                  fib_heap_node* const node2);

static void
FibHeapLink (fib_heap_node* const y,
             fib_heap_node* const x);

static fib_heap_node*
FibHeapFromRootsArray (fib_heap_node** const roots_array,
                       const size_t roots_max_num);

fib_heap*
FibHeapConstructor ()
{
    fib_heap* const heap =
        (fib_heap*) calloc (1, sizeof (fib_heap));
    
    return heap;
}

fib_heap*
FibHeapDestructor (fib_heap* const heap)
{
    if (heap == NULL) return NULL;

    while (heap->min_elem != NULL)
        FibHeapExtractMin (heap);

    free (heap);
    return NULL;
}

fib_heap_node*
FibHeapNodeConstructor (const int key)
{
    fib_heap_node* const node =
        (fib_heap_node*) calloc (1, sizeof (fib_heap_node));
    if (node == NULL) return NULL;

    node->key  = key;
    node->mark = false;

    return node;
}

fib_heap_node*
FibHeapNodeDestructor (fib_heap_node* const node)
{
    if (node == NULL) return NULL;

    free (node);
    return NULL;
}

int
FibHeapGetMin (const fib_heap* const heap)
{
    if (heap == NULL ||
        heap->size == 0)
        return FIB_HEAP_POISON;

    return heap->min_elem->key; 
}

fib_heap_error_t
FibHeapExtractMin (fib_heap* const heap)
{
    if (heap == NULL || heap->min_elem == NULL)
        return FIB_HEAP_ERROR;

    fib_heap_node* const prev_min = heap->min_elem;

    MakeNullParentsForChildren (prev_min);
    UnionCyclicLists (heap->min_elem, heap->min_elem->child);

    prev_min->left->right = prev_min->right;
    prev_min->right->left = prev_min->left;

    if (prev_min->right == prev_min)
        heap->min_elem = NULL;

    else
    {
        heap->min_elem = prev_min->right;
        FibHeapConsolidate (heap);
    }

    FibHeapNodeDestructor (prev_min);
    heap->size--;

    return FIB_HEAP_SUCCESS;
}

fib_heap_node*
FibHeapInsert (fib_heap* const heap,
               const int key)
{
    if (heap == NULL) return NULL;

    fib_heap_node* const new_node = FibHeapNodeConstructor (key);
    if (new_node == NULL) return NULL;

    fib_heap_node* prev_right = NULL;

    if (heap->size == 0)
    {
        new_node->left  = new_node;
        new_node->right = new_node;
        heap->min_elem  = new_node;
    }

    else
    {
        prev_right = heap->min_elem->right;

        heap->min_elem->right = new_node;
        new_node->left = heap->min_elem;
        
        prev_right->left = new_node;
        new_node->right = prev_right;
    }

    if (new_node->key < heap->min_elem->key)
        heap->min_elem = new_node;

    heap->size++;

    return new_node;
}

fib_heap_error_t
FibHeapDecreaseKey (fib_heap* const heap,
                    fib_heap_node* const node,
                    const int key)
{
    if (heap == NULL ||
        node == NULL)
        return FIB_HEAP_ERROR;

    node->key = key;

    if (node->parent == NULL ||
        key > node->parent->key)
        return FIB_HEAP_SUCCESS;

    fib_heap_node* parent = node->parent;
    FibHeapCut (heap, node);
    FibHeapCascadingCut (heap, parent);

    return FIB_HEAP_SUCCESS;
}

fib_heap_error_t
FibHeapDeleteKey (fib_heap* const heap,
                  fib_heap_node* const node)
{
    if (heap == NULL ||
        node == NULL ||
        heap->size == 0)
        return FIB_HEAP_ERROR;

    if (FibHeapDecreaseKey (heap, node, INT_MIN) == FIB_HEAP_ERROR)
        return FIB_HEAP_ERROR;

    return FibHeapExtractMin (heap);
}

static fib_heap_node*
UnionCyclicLists (fib_heap_node* const first,
                  fib_heap_node* const second)
{
    if (first  == NULL) return second;
    if (second == NULL) return first;

    fib_heap_node* const left_node  = first->left;
    fib_heap_node* const right_node = second->right;

    second->right = first;
    first->left   = second;
    left_node->right = right_node;
    right_node->left = left_node;

    return first;
}

void
FibHeapConsolidate (fib_heap* const heap)
{
    if (heap           == NULL ||
        heap->min_elem == NULL) 
        return;

    fib_heap_node** const nodes_array = 
        (fib_heap_node**) calloc (heap->size, sizeof (fib_heap_node*));
    if (nodes_array == NULL) return;

    fib_heap_node* cur  = heap->min_elem;
    fib_heap_node* next = NULL;
    fib_heap_node* x    = NULL;
    fib_heap_node* y    = NULL;
    size_t d = 0;
    
    // fib_heap_node* print_node = heap->min_elem;

    // for (size_t i = 0; i < heap->size; i++)
    // {
    //     fprintf (stderr, "%d ", print_node->key);
    //     print_node = print_node->right;
    // }

    do
    {
        x = cur;
        next = cur->right;
        cur->right = cur;
        cur->left  = cur;

        // fprintf (stderr, "cur %d next %d\n", cur->key, next->key);

        d = x->degree;

        while (nodes_array[d] != NULL)
        {
            y = nodes_array[d];

            // fprintf (stderr, "x %d, y %d\n", x->key, y->key);

            if (x->key > y->key)
                FibHeapSwapNodes (x, y);

            FibHeapLink (y, x);
            nodes_array[d] = NULL;
            ++d;
        }

        nodes_array[d] = x;
        cur = next;
    } 
    while (cur != heap->min_elem);

    fprintf (stderr, "\n%d\n", heap->min_elem->key);

    heap->min_elem = FibHeapFromRootsArray (nodes_array, heap->size);

    free (nodes_array);
}

static void
MakeNullParentsForChildren (fib_heap_node* const node)
{
    if (node == NULL) return;

    fib_heap_node* cur = node->child;
    if (cur == NULL) return;

    do
    {
        cur->parent = NULL;
        cur = cur->right;
    }
    while (cur != node->child);
}

static void
FibHeapLink (fib_heap_node* const y,
             fib_heap_node* const x)
{
    if (x->child == NULL)
    {
        x->child  = y;
        y->parent = x;
        y->left   = y;
        y->right  = y;
    }

    else
    {
        y->parent = x;
        y->left->right = y->right;
        y->right->left = y->left;
        y->right = x->child;
        x->child->right->left = y;
        x->child->right = y;
    }

    y->mark = false;
    ++x->degree;
}

static fib_heap_node*
FibHeapFromRootsArray (fib_heap* const heap,
                       fib_heap_node** const roots_array,
                       const size_t roots_max_num)
{
    if (heap        == NULL ||
        roots_array == NULL)
        return NULL;

    heap->min_elem = NULL;

    fib_heap_node* cur  = roots_array[0];
    fib_heap_node* prev = NULL;

    for (size_t i = 0; i < roots_max_num; ++i)
    {
        cur = roots_array[i];
        if (cur != NULL)
        {
            if (heap->min_elem == NULL)
                heap->min_elem = cur;

            else
            {
                heap->min
            }
        }
    }

    return min;
}

static void
FibHeapCut (fib_heap* const heap,
            fib_heap_node* const node)
{
    if (heap == NULL ||
        node == NULL)
        return;

    fib_heap_node* const left_node  = node->left;
    fib_heap_node* const right_node = node->right;

    right_node->left = left_node;
    left_node->right = right_node;
    node->parent->degree--;

    if (node->parent->child == node)
    {
        if (node->right == node)
            node->parent->child = NULL;
        else
            node->parent->child = node->right;
    }

    node->right  = node;
    node->left   = node;
    node->parent = NULL;
    heap->min_elem = UnionCyclicLists (heap->min_elem, node);
}

static void
FibHeapCascadingCut (fib_heap* const heap,
                     fib_heap_node*  node)
{
    if (heap == NULL ||
        node == NULL)
        return;

    while (node != NULL &&
           node->mark == true)
    {
        FibHeapCut (heap, node);
        node = node->parent;
    }

    if (node) node->mark = true;
}

static void
FibHeapSwapNodes (fib_heap_node* const node1,
                  fib_heap_node* const node2)
{
    if (node1 == NULL ||
        node2 == NULL)
        return;

    fib_heap_node temp = *node1;
    *node1 = *node2;
    *node2 = temp;
}