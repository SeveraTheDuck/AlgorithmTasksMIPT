#include "kary_heap.h"
#include <stdio.h>

const size_t K_HEAP_NULL_SIZE  = 0;
const size_t K_HEAP_ROOT_INDEX = 0;
const size_t K_HEAP_INITIAL_DATA_ARRAY_CAPACITY = 4;

static void
swap (void* elem1, void* elem2, const size_t elem_size);

struct k_heap*
KHeapConstructor (struct dynamic_array* d_array,
                  const  size_t k,
                  const  size_t elem_size,
                  const  void* const infinity_value,
                  int    (*comparator) (void*, void*))
{
    struct k_heap* heap =
        (struct k_heap*) calloc (1, sizeof (struct k_heap));
    if (heap == NULL) return NULL;

    heap->k = k;
    heap->infinity_value = infinity_value;
    heap->cmp = comparator;

    if (d_array == NULL)
    {
        heap->d_array =
            DynamicArrayConstructor (K_HEAP_INITIAL_DATA_ARRAY_CAPACITY, elem_size,
                                     DYNAMIC_ARRAY_REALLOC_ENABLED,
                                     DYNAMIC_ARRAY_DESTROY);
        if (heap->d_array == NULL)
        {
            free (heap);
            return NULL;
        }
    }

    else
    {
        heap->d_array = d_array;

        for (size_t i = d_array->data_array_size / heap->k; i > 0; --i)
            KHeapSiftDown (heap, i);

        KHeapSiftDown (heap, K_HEAP_ROOT_INDEX);
    }

    return heap;
}

struct k_heap*
KHeapDestructor (struct k_heap* const heap)
{
    if (heap == NULL) return NULL;

    heap->d_array = DynamicArrayDestructor (heap->d_array);
    heap->infinity_value = NULL;
    heap->k = K_HEAP_NULL_SIZE;
    heap->cmp = NULL;

    return NULL;
}

k_heap_error_t
KHeapSiftUp (struct k_heap* const heap,
             const size_t key_index)
{
    if (heap == NULL || key_index >= heap->d_array->data_array_size)
        return K_HEAP_ERROR;

    size_t  cur_index  = key_index;
    size_t  parent     = 0;
    size_t* cur_ptr    = NULL;
    size_t* parent_ptr = NULL;

    while (cur_index > 0)
    {
        parent = (cur_index - 1) / heap->k;

        cur_ptr    = KHeapGetElemPtrByIndex (heap, cur_index);
        parent_ptr = KHeapGetElemPtrByIndex (heap, parent);

        if (cur_ptr == NULL || parent_ptr == NULL)
            return K_HEAP_ERROR;

        if (heap->cmp (parent_ptr, cur_ptr) < 0)
        {
            swap (parent_ptr, cur_ptr, heap->d_array->elem_size);
            cur_index = parent;
        }

        else return K_HEAP_SUCCESS;
    }

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapSiftDown (struct k_heap* const heap,
               const size_t key_index)
{
    if (heap == NULL || key_index >= heap->d_array->data_array_size)
        return K_HEAP_ERROR;

    size_t cur_index = key_index;
    size_t cur_child = key_index * heap->k + 1;

    size_t* cur_ptr = KHeapGetElemPtrByIndex (heap, cur_index);
    size_t* cur_child_ptr = NULL;

    for (size_t i = 0; i < heap->k; ++i)
    {
        cur_child_ptr = KHeapGetElemPtrByIndex (heap, cur_child);
        if (cur_child_ptr == NULL || cur_child >= heap->d_array->data_array_size)
            break;

        if (heap->cmp (cur_ptr, cur_child_ptr) < 0)
        {
            cur_index     = cur_child;
            cur_ptr       = cur_child_ptr;
        }

        cur_child++;
    }

    if (cur_index != key_index)
    {
        swap (KHeapGetElemPtrByIndex (heap, key_index),
              cur_ptr, heap->d_array->elem_size);
        KHeapSiftDown (heap, cur_index);
    }

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapInsert (struct k_heap* const heap,
             const  void*   const insert_buffer)
{
    if (heap == NULL || insert_buffer == NULL)
        return K_HEAP_ERROR;

    if (DynamicArrayPush (heap->d_array, insert_buffer) == DYNAMIC_ARRAY_ERROR)
        return K_HEAP_ERROR;

    if (KHeapSiftUp (heap, heap->d_array->data_array_size - 1) == K_HEAP_ERROR)
        return K_HEAP_ERROR;

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapExtractRoot (struct k_heap* const heap,
                         void*   const get_buffer)
{
    if (heap == NULL)
        return K_HEAP_ERROR;

    void* const root_ptr = KHeapGetElemPtrByIndex
        (heap, K_HEAP_ROOT_INDEX);

    void* const last_ptr = KHeapGetElemPtrByIndex
        (heap, heap->d_array->data_array_size - 1);

    // fprintf (stderr, "LAST_PTR %d\n", *(int*)last_ptr);

    if (root_ptr == NULL || last_ptr == NULL) return K_HEAP_ERROR;

    if (get_buffer != NULL)
        memcpy (get_buffer, root_ptr, heap->d_array->elem_size);

    memcpy (root_ptr, last_ptr, heap->d_array->elem_size);

    if (DynamicArrayPop (heap->d_array) == DYNAMIC_ARRAY_ERROR)
        return K_HEAP_ERROR;

    if (KHeapSiftDown (heap, K_HEAP_ROOT_INDEX) == K_HEAP_ERROR)
        return K_HEAP_ERROR;

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapDeleteKey (struct k_heap* const heap,
                const size_t key_index)
{
    if (heap == NULL || key_index >= heap->d_array->data_array_size)
        return K_HEAP_ERROR;

    // a[key_index] = inf;
    memcpy (KHeapGetElemPtrByIndex (heap, key_index),
            heap->infinity_value, heap->d_array->elem_size);

    if (KHeapSiftUp (heap, key_index) == K_HEAP_ERROR)
        return K_HEAP_ERROR;

    if (KHeapExtractRoot (heap, NULL) == K_HEAP_ERROR)
        return K_HEAP_ERROR;

    return K_HEAP_SUCCESS;
}

void*
KHeapGetElemPtrByIndex (struct k_heap* const heap,
                        const size_t key_index)
{
    if (heap == NULL) return NULL;

    return DynamicArrayGetElemPtrByIndex (heap->d_array, key_index);
}

static void
swap (void* elem1, void* elem2, const size_t elem_size)
{
    assert (elem1);
    assert (elem2);

    void* tmp_elem = calloc (1, elem_size);
    assert (tmp_elem);

    memcpy (tmp_elem, elem2,    elem_size);
    memcpy (elem2,    elem1,    elem_size);
    memcpy (elem1,    tmp_elem, elem_size);
}