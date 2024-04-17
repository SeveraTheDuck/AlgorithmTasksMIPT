#include "../include/fib_heap.h"



const int FIB_HEAP_POISON = INT_MAX;
#define GOLDEN_RATIO (1.0 + sqrt (5)) / 2.0



static void
FibHeapConsolidate (fib_heap* const heap);

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
FibHeapSwapNodes (fib_heap_node** const node1,
                  fib_heap_node** const node2);

static void
FibHeapLink (fib_heap_node* const y,
             fib_heap_node* const x);

static void
FibHeapCutNode (fib_heap_node* const node);

static void
FibHeapFromRootsArray (fib_heap* const heap,
                       fib_heap_node** const roots_array,
                       const size_t roots_max_num);

static size_t
FibHeapMaxDegree (const size_t elem_number);

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

    node->key   = key;
    node->left  = node;
    node->right = node;

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
    if (heap       == NULL ||
        heap->size == 0)
        return FIB_HEAP_POISON;

    return heap->min_elem->key;
}

fib_heap_error_t
FibHeapExtractMin (fib_heap* const heap)
{
    if (heap           == NULL ||
        heap->min_elem == NULL)
        return FIB_HEAP_ERROR;

    fib_heap_node* const prev_min = heap->min_elem;

    MakeNullParentsForChildren (prev_min);
    UnionCyclicLists (prev_min, prev_min->child);

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

    heap->min_elem = UnionCyclicLists (heap->min_elem, new_node);

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

    if (node->key < heap->min_elem->key)
        heap->min_elem = node;

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

    second->right    = first;
    first->left      = second;
    left_node->right = right_node;
    right_node->left = left_node;

    return first;
}

static void
FibHeapConsolidate (fib_heap* const heap)
{
    if (heap           == NULL ||
        heap->min_elem == NULL)
        return;

    const size_t roots_array_size = FibHeapMaxDegree (heap->size);

    fib_heap_node** const roots_array =
        (fib_heap_node**) calloc (roots_array_size, sizeof (fib_heap_node*));
    if (roots_array == NULL) return;

    fib_heap_node* cur  = heap->min_elem;
    fib_heap_node* next = NULL;
    fib_heap_node* x    = NULL;
    fib_heap_node* y    = NULL;
    size_t d = 0;

    do
    {
        x = cur;
        next = cur->right;

        d = x->degree;
        while (roots_array[d] != NULL)
        {
            y = roots_array[d];

            if (x->key > y->key)
                FibHeapSwapNodes (&x, &y);

            FibHeapLink (y, x);

            roots_array[d] = NULL;
            ++d;
        }

        roots_array[d] = x;
        cur = next;
    }
    while (cur != heap->min_elem);

    FibHeapFromRootsArray (heap, roots_array, roots_array_size);

    free (roots_array);
}

static void
MakeNullParentsForChildren (fib_heap_node* const node)
{
    if (node == NULL) return;

    fib_heap_node* const first_child = node->child;
    fib_heap_node* cur = first_child;
    if (cur == NULL) return;

    do
    {
        cur->parent = NULL;
        cur = cur->right;
    }
    while (cur != first_child);
}

static void
FibHeapLink (fib_heap_node* const y,
             fib_heap_node* const x)
{
    if (y == NULL ||
        x == NULL)
        return;

    FibHeapCutNode (y);

    x->child = UnionCyclicLists (x->child, y);

    y->parent = x;
    ++x->degree;

    y->mark = false;
}

static void
FibHeapCutNode (fib_heap_node* const node)
{
    if (node == NULL) return;

    node->left->right = node->right;
    node->right->left = node->left;
    node->left        = node;
    node->right       = node;
}

static void
FibHeapFromRootsArray (fib_heap* const heap,
                       fib_heap_node** const roots_array,
                       const size_t roots_max_num)
{
    if (heap        == NULL ||
        roots_array == NULL)
        return;

    heap->min_elem = NULL;

    fib_heap_node* cur = NULL;

    for (size_t i = 0; i < roots_max_num; ++i)
    {
        cur = roots_array[i];

        if (cur != NULL)
        {
            cur->left   = cur;
            cur->right  = cur;
            cur->parent = NULL;

            heap->min_elem = UnionCyclicLists (heap->min_elem, cur);
            if (cur->key < heap->min_elem->key)
            {
                heap->min_elem = cur;
            }
        }
    }
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
FibHeapSwapNodes (fib_heap_node** const node1,
                  fib_heap_node** const node2)
{
    if (node1 == NULL ||
        node2 == NULL)
        return;

    fib_heap_node* const temp = *node1;
    *node1 = *node2;
    *node2 = temp;
}

static size_t
FibHeapMaxDegree (const size_t elem_number)
{
    if (elem_number == 0) return 0;

    return (size_t) (log ((double)elem_number) / log (GOLDEN_RATIO) + 1);
}