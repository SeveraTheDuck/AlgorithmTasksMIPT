#include "../include/binomial_heap.h"



static binomial_heap_node*
BinomialHeapMergeRoots (binomial_heap_node* const list1,
                        binomial_heap_node* const list2);

static binomial_heap_node*
BinomialHeapLinkTrees (binomial_heap_node* const tree1,
                       binomial_heap_node* const tree2);



binomial_heap_node*
BinomialHeapDestructor (binomial_heap_node* const heap)
{
    if (heap == NULL) return NULL;

    heap->sibling = BinomialHeapDestructor (heap->sibling);
    heap->child   = BinomialHeapDestructor (heap->child);

    return BinomialHeapNodeDestructor (heap);
}

binomial_heap_node*
BinomialHeapMerge (binomial_heap_node* const heap1,
                   binomial_heap_node* const heap2)
{
    if (heap1 == NULL) return heap2;
    if (heap2 == NULL) return heap1;

    binomial_heap_node* const head =
        BinomialHeapMergeRoots (heap1, heap2);
    if (head == NULL) return NULL;

    binomial_heap_node* prev = NULL;
    binomial_heap_node* cur  = head;
    binomial_heap_node* next = head->sibling;

    binomial_heap_node* merge_result = head;

    while (next != NULL)
    {
        if (cur->degree != next->degree ||
            next->sibling != NULL && next->sibling->degree == cur->degree)
        {
            prev = cur;
            cur = next;
        }

        else if (cur->key < next->key)
        {
            cur->sibling = next->sibling;
            BinomialHeapLinkTrees (next, cur);
        }

        else
        {
            if (prev == NULL) merge_result = next;
            else prev->sibling = next;

            BinomialHeapLinkTrees (cur, next);
            cur = next;
        }

        next = cur->sibling;
    }

    return merge_result;
}

binomial_heap_node*
BinomialHeapInsert (binomial_heap_node* const heap,
                    const int key)
{
    binomial_heap_node* const new_node =
        BinomialHeapNodeConstructor (key);
    if (new_node == NULL) return NULL;

    if (heap == NULL) return new_node;

    return BinomialHeapMerge (heap, new_node);
}

binomial_heap_node*
BinomialHeapNodeConstructor (const int key)
{
    binomial_heap_node* const node =
        (binomial_heap_node*) calloc (1, sizeof (binomial_heap_node));
    if (node == NULL) return NULL;

    node->key     = key;
    node->parent  = NULL;
    node->child   = NULL;
    node->sibling = NULL;
    node->degree  = 0;

    return node;
}

binomial_heap_node*
BinomialHeapNodeDestructor (binomial_heap_node* const node)
{
    if (node == NULL) return NULL;

    free (node);
    return NULL;
}

static binomial_heap_node*
BinomialHeapMergeRoots (binomial_heap_node* const list1,
                        binomial_heap_node* const list2)
{
    if (list1 == NULL) return list2;
    if (list2 == NULL) return list1;

    binomial_heap_node* head = list1->degree <= list2->degree ? list1 : list2;

    binomial_heap_node* cur1  = NULL;
    binomial_heap_node* cur2  = NULL;
    binomial_heap_node* next1 = list1;
    binomial_heap_node* next2 = list2;

    while (next1 != NULL && next2 != NULL)
    {
        cur1 = next1;
        cur2 = next2;

        if (cur1->degree <= cur2->degree)
        {
            next1 = cur1->sibling;
            cur1->sibling = cur2;
        }

        else
        {
            next2 = cur2->sibling;
            cur2->sibling = cur1;
        }
    }

    return head;
}

static binomial_heap_node*
BinomialHeapLinkTrees (binomial_heap_node* const tree1,
                       binomial_heap_node* const tree2)
{
    if (tree1 == NULL) return tree2;
    if (tree2 == NULL) return tree1;

    if (tree1->degree != tree2->degree) return NULL;

    tree1->parent  = tree2;
    tree1->sibling = tree2->child;
    tree2->child   = tree1;
    tree2->degree++;

    return tree2;
}
