#include "../include/k_heap.h"

#include <stdio.h>



//-----------------------------------------------------------------------------
// K-heap static functions prototypes
//-----------------------------------------------------------------------------
static k_heap_error_t
KHeapPushNewItem (k_heap_t*         const heap,
                  const k_heap_key* const key);

static k_heap_error_t
KHeapPushNewItemNoRealloc (k_heap_t*         const heap,
                           const k_heap_key* const key);

static k_heap_error_t
KHeapCopyLastElement (k_heap_t* const heap);

static k_heap_error_t
KHeapPopLastElement (k_heap_t* const heap);

static k_heap_error_t
KHeapSiftUp (k_heap_t* const heap,
             const size_t index);

static k_heap_key*
KHeapKeyCopy (const k_heap_key* const key);

static size_t
KHeapSize (const k_heap_t* const heap);

static void
KHeapSwap (k_heap_t* const heap,
           const size_t index_1,
           const size_t index_2);
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-heap functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
k_heap_t*
KHeapConstructor (const size_t k,
                  const k_heap_key* const infinity_key,
                  int (*key_cmp) (const k_heap_key* const,
                                  const k_heap_key* const))
{
    if (infinity_key == NULL ||
        key_cmp      == NULL)
        return NULL;

    k_heap_t* const heap = (k_heap_t*) malloc (sizeof (k_heap_t));
    if (heap == NULL)
        return NULL;

    heap->k            = k;
    heap->infinity_key = KHeapKeyCopy (infinity_key);
    heap->array        = DynamicArrayConstructor (sizeof (k_heap_key*));
    heap->key_cmp      = key_cmp;

    return heap;
}

k_heap_t*
KHeapDestructor  (k_heap_t* const heap)
{
    if (heap == NULL)
        return NULL;

    heap->infinity_key = KHeapKeyDestructor (heap->infinity_key);

    k_heap_key** cur_key = NULL;
    const size_t elem_number = KHeapSize (heap);

    for (size_t i = 0; i < elem_number; ++i)
    {
        cur_key = DynamicArrayGetElemPtrByIndex (heap->array, i);
        
        if (cur_key != NULL)
            *cur_key = KHeapKeyDestructor (*cur_key);
    }

    heap->array = DynamicArrayDestructor (heap->array);

    free (heap);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Interface
k_heap_error_t
KHeapInsert (k_heap_t*         const heap,
             const k_heap_key* const key)
{
    bool error_status = K_HEAP_SUCCESS;

    error_status = error_status || KHeapPushNewItem (heap, key);

    error_status = error_status || KHeapSiftUp (heap, KHeapSize (heap) - 1);

    return error_status;
}

k_heap_error_t
KHeapInsertNoRealloc (k_heap_t*         const heap,
                      const k_heap_key* const key)
{
    bool error_status = K_HEAP_SUCCESS;

    error_status = error_status || KHeapPushNewItemNoRealloc (heap, key);

    error_status = error_status || KHeapSiftUp (heap, KHeapSize (heap) - 1);

    return error_status;
}

k_heap_key*
KHeapGetRoot (k_heap_t* const heap)
{
    if (heap == NULL) return NULL;

    k_heap_key** const key_ptr =
        DynamicArrayGetElemPtrByIndex (heap->array, 0);

    if (key_ptr == NULL) return NULL;
    return *key_ptr;
}

k_heap_error_t
KHeapExtractRoot (k_heap_t* const heap)
{
    bool error_status = K_HEAP_SUCCESS;

    error_status = error_status || KHeapCopyLastElement (heap);

    error_status = error_status || KHeapPopLastElement  (heap);

    error_status = error_status || KHeapSiftDown (heap, 0);

    return error_status;
}
//-------------------------------------

//-------------------------------------
// Static functions
static k_heap_error_t
KHeapPushNewItem (k_heap_t*         const heap,
                  const k_heap_key* const key)
{
    if (heap  == NULL ||
        key   == NULL)
        return K_HEAP_ERROR;

    k_heap_key* new_key = KHeapKeyCopy (key);
    if (new_key == NULL) return K_HEAP_ERROR;

    if (DynamicArrayPush (heap->array, &new_key) == DYNAMIC_ARRAY_ERROR)
    {
        new_key = KHeapKeyDestructor (new_key);
        return K_HEAP_ERROR;
    }

    return K_HEAP_SUCCESS;
}

static k_heap_error_t
KHeapPushNewItemNoRealloc (k_heap_t*         const heap,
                           const k_heap_key* const key)
{
    if (heap  == NULL ||
        key   == NULL)
        return K_HEAP_ERROR;

    k_heap_key* new_key = KHeapKeyCopy (key);
    if (new_key == NULL) return K_HEAP_ERROR;

    if (DynamicArrayPushNoRealloc (heap->array, &new_key) == DYNAMIC_ARRAY_ERROR)
    {
        new_key = KHeapKeyDestructor (new_key);
        return K_HEAP_ERROR;
    }

    return K_HEAP_SUCCESS;
}

static k_heap_error_t
KHeapCopyLastElement (k_heap_t* const heap)
{
    if (heap        == NULL ||
        heap->array == NULL)
        return K_HEAP_ERROR;

    k_heap_key** const last_key_ptr =
        DynamicArrayGetElemPtrByIndex (heap->array, KHeapSize (heap) - 1);
    if (last_key_ptr == NULL) return K_HEAP_ERROR;

    k_heap_key* const last_key = *last_key_ptr;
    if (last_key == NULL) return K_HEAP_ERROR;

    k_heap_key** const root_key_ptr =
        DynamicArrayGetElemPtrByIndex (heap->array, 0);
    if (root_key_ptr == NULL) return K_HEAP_ERROR;

    k_heap_key* const root_key = *root_key_ptr;
    if (root_key == NULL) return K_HEAP_ERROR;

    if (root_key == last_key) return K_HEAP_SUCCESS;

    KHeapKeyDestructor (root_key);
    *root_key_ptr = KHeapKeyCopy (last_key);

    return K_HEAP_SUCCESS;
}

static k_heap_error_t
KHeapPopLastElement (k_heap_t* const heap)
{
    if (heap == NULL) return K_HEAP_ERROR;
    
    k_heap_key** const last_key_ptr =
        DynamicArrayGetElemPtrByIndex (heap->array, KHeapSize (heap) - 1);
    if (last_key_ptr == NULL) return K_HEAP_ERROR;

    k_heap_key* const last_key = *last_key_ptr;
    if (last_key == NULL) return K_HEAP_ERROR;

    KHeapKeyDestructor (last_key);

    d_array_error_t error_status = DynamicArrayPop (heap->array);

    if (error_status == DYNAMIC_ARRAY_ERROR) return K_HEAP_ERROR;
    return K_HEAP_SUCCESS;
}

static k_heap_error_t
KHeapSiftUp (k_heap_t* const heap,
             const size_t index)
{
    if (heap          == NULL ||
        heap->array   == NULL ||
        heap->key_cmp == NULL)
        return K_HEAP_ERROR;

    size_t cur_index    = index;
    size_t parent_index = 0;

    k_heap_key** cur_key_ptr    = NULL;
    k_heap_key** parent_key_ptr = NULL;

    while (cur_index != 0)
    {
        parent_index = (cur_index - 1) / 2;

        cur_key_ptr    = DynamicArrayGetElemPtrByIndex (heap->array, cur_index);
        parent_key_ptr = DynamicArrayGetElemPtrByIndex (heap->array, parent_index);

        if (heap->key_cmp (*parent_key_ptr, *cur_key_ptr) == K_HEAP_CMP_LESS)
        {
            KHeapSwap (heap, parent_index, cur_index);
            cur_index = parent_index;
        }

        else break;
    }

    return K_HEAP_SUCCESS;
}

k_heap_error_t
KHeapSiftDown (k_heap_t* const heap,
               const size_t index)
{
    if (heap          == NULL ||
        heap->key_cmp == NULL)
        return K_HEAP_ERROR;

    const size_t heap_size = KHeapSize (heap);

    if (index >= heap_size) return K_HEAP_SUCCESS;

    size_t min_index         = index;
    size_t left_child_index  = index * 2 + 1;
    size_t right_child_index = index * 2 + 2;

    k_heap_key** min_key   =
        DynamicArrayGetElemPtrByIndex (heap->array, min_index);

    k_heap_key** left_key  = 
        DynamicArrayGetElemPtrByIndex (heap->array, left_child_index);

    k_heap_key** right_key = 
        DynamicArrayGetElemPtrByIndex (heap->array, right_child_index);

    if (left_child_index < heap_size && left_key != NULL &&
        heap->key_cmp (*min_key, *left_key) == K_HEAP_CMP_LESS)
        min_index = left_child_index;

    min_key = DynamicArrayGetElemPtrByIndex (heap->array, min_index);

    if (right_child_index < heap_size && right_key != NULL &&
        heap->key_cmp (*min_key, *right_key) == K_HEAP_CMP_LESS)
        min_index = right_child_index;

    if (min_index != index)
    {
        KHeapSwap (heap, min_index, index);
        return KHeapSiftDown (heap, min_index);
    }

    return K_HEAP_SUCCESS;
}

static size_t
KHeapSize (const k_heap_t* const heap)
{
    if (heap        == NULL ||
        heap->array == NULL)
        return 0;

    return heap->array->size;
}

static void
KHeapSwap (k_heap_t* const heap,
           const size_t index_1,
           const size_t index_2)
{
    if (heap        == NULL ||
        heap->array == NULL)
        return;

    k_heap_key** const key_ptr_1 =
        DynamicArrayGetElemPtrByIndex (heap->array, index_1);
    if (key_ptr_1 == NULL) return;

    k_heap_key** const key_ptr_2 =
        DynamicArrayGetElemPtrByIndex (heap->array, index_2);
    if (key_ptr_2 == NULL) return;

    k_heap_key* const tmp_key = *key_ptr_1;

    *key_ptr_1 = *key_ptr_2;
    *key_ptr_2 = tmp_key;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// K-heap key and value functions implementation
//-----------------------------------------------------------------------------
//-------------------------------------
// Constructor and destructor
k_heap_key*
KHeapKeyConstructor (const void* const key,
                     const size_t key_len)
{
    k_heap_key* const new_key =
        (k_heap_key*) calloc (1, sizeof (k_heap_key));
    if (new_key == NULL)
        return NULL;

    if (key != NULL && key_len != 0)
    {
        new_key->key = malloc (key_len);
        if (new_key->key == NULL)
            return KHeapKeyDestructor (new_key);

        memcpy (new_key->key, key, key_len);
        new_key->key_len = key_len;
    }

    return new_key;
}

k_heap_key*
KHeapKeyDestructor (k_heap_key* const key)
{
    if (key == NULL) return NULL;

    free (key->key);
    free (key);
    return NULL;
}
//-------------------------------------

//-------------------------------------
// Static functions
static k_heap_key*
KHeapKeyCopy (const k_heap_key* const key)
{
    if (key == NULL) return NULL;
    return KHeapKeyConstructor (key->key, key->key_len);
}
//-------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------